#include "note_canvas.hpp"

#include <algorithm>
#include <string>

#include <raygui.h>

#include "constants.hpp"
#include "utilities/project_utilities.hpp"

using namespace interface;
using namespace constants::interface_layout::note_canvas;

void NoteCanvas::drawCommands(program_states::InterfaceContext &context){

	const auto projectData{utilities::projectDataWithPagesFrom(context.system)};
	if(!projectData) return;

	auto &noteCanvasState{context.interface.noteCanvas};
	const auto &sidebarState{context.interface.sidebar};

	const int selectedChannelListViewIndex{sidebarState.selectedChannelListViewIndex};
	const bool isSystemChannelSelection{
		selectedChannelListViewIndex == notes::SystemChannelListViewIndex
	};
	const bool shouldShowCommandsEverywhere{sidebarState.isShowCommandsEnabled};
	const bool shouldDrawCommands{isSystemChannelSelection || shouldShowCommandsEverywhere};
	if(!shouldDrawCommands) return;

	const float commandAlpha{
		isSystemChannelSelection
			? commands::FullVisibilityAlpha
			: commands::TransparentVisibilityAlpha
	};


	const int currentPageIndex{utilities::currentPageIndexFrom(*projectData, context.system.project.currentPage)};
	const auto &currentPage{projectData->pages[currentPageIndex]};
	const int currentPageNoteCount{utilities::currentPageNoteCountFrom(*projectData, context.system.project.currentPage)};

	const float topY{rowEdgeY(context, 0)};
	const float bottomY{rowEdgeY(context, NumberOfRow)};
	const float commandHeight{bottomY - topY};


	for(int noteColumnIndex{0}; noteColumnIndex < currentPageNoteCount; noteColumnIndex++){
		
		const auto &commandTokenCell{currentPage.commandChannel[noteColumnIndex]};
		if(!commandTokenCell.has_value()) continue;

		const auto commandData{NoteCanvas::commandFromToken(commandTokenCell.value())};
		if(!commandData) continue;

		const Rectangle columnBounds{
			noteCanvasState.gridArea.x + (static_cast<float>(noteColumnIndex) * noteCanvasState.columnWidth),
			topY,
			noteCanvasState.columnWidth,
			commandHeight
		};

		const float textWidth{std::max(.0f, columnBounds.width - (commands::HorizontalPadding * 2.0f))};
		const auto commandVisualDescriptor{
			NoteCanvas::createCommandBigNote(commandData.value(), textWidth, commands::TextFontSize)
		};

		const Color fillColor{Fade(ColorBrightness(commandVisualDescriptor.baseColor, -notes::InstrumentIndexColorDarkeningFactor), commandAlpha)};
		const Color borderColor{Fade(notes::NoteBorderColor, commandAlpha)};
		const Color textColor{Fade(WHITE, commandAlpha)};


		DrawRectangleRec(columnBounds, fillColor);
		DrawRectangleLinesEx(columnBounds, notes::BorderThicknessInPixels, borderColor);

		const std::string trimmedFirstTextLine{NoteCanvas::trimTextToFitWidth(commandVisualDescriptor.firstTextLine, textWidth, commands::TextFontSize)};
		const std::string trimmedSecondTextLine{NoteCanvas::trimTextToFitWidth(commandVisualDescriptor.secondTextLine, textWidth, commands::TextFontSize)};
		const std::string trimmedThirdTextLine{NoteCanvas::trimTextToFitWidth(commandVisualDescriptor.thirdTextLine, textWidth, commands::TextFontSize)};

		const float stringCenterY{columnBounds.y + (commandHeight * commands::ConstantStringCenterRatio)};
		const float iconCenterY{columnBounds.y + (commandHeight * commands::IconCenterRatio)};
		const float nameCenterY{columnBounds.y + (commandHeight * commands::NameCenterRatio)};
		const float valueCenterY{columnBounds.y + (commandHeight * commands::ValueCenterRatio)};
		const float targetCenterY{columnBounds.y + (commandHeight * commands::TargetCenterRatio)};

		// TODO: command constants
		NoteCanvas::drawCenteredTextLine(columnBounds, stringCenterY, commands::EmptyText, textColor);

		GuiDrawIcon(
			commandVisualDescriptor.iconIndex,
			columnBounds.x + (columnBounds.width - IconSize) * .5f,
			iconCenterY - (IconSize * .5f),
			1,
			textColor
		);

		NoteCanvas::drawCenteredTextLine(columnBounds, nameCenterY, trimmedFirstTextLine, textColor);
		NoteCanvas::drawCenteredTextLine(columnBounds, valueCenterY, trimmedSecondTextLine, textColor);
		NoteCanvas::drawCenteredTextLine(columnBounds, targetCenterY, trimmedThirdTextLine, textColor);

	}



}

std::optional<command::Command> NoteCanvas::commandFromToken(const command::CommandToken &token){
	if(!std::holds_alternative<command::Command>(token)) return std::nullopt;
	return std::get<command::Command>(token);

}

bool NoteCanvas::doesTextFitWidth(const std::string &text, float width, int fontSize){
	if(text.empty() || width <= .0f) return false;

	const auto textSize{MeasureTextEx(GetFontDefault(), text.c_str(), static_cast<float>(fontSize), 1.0f)};
	return textSize.x <= width;
}

std::string NoteCanvas::trimTextToFitWidth(const std::string &text, float width, int fontSize){
	if(text.empty() || width <= 1.0f) return "";

	std::string trimmed{text};
	while(!trimmed.empty()){
		const auto textSize{MeasureTextEx(GetFontDefault(), trimmed.c_str(), static_cast<float>(fontSize), 1.0f)};
		if(textSize.x <= width) return trimmed;

		trimmed.pop_back();
	}

	return "";

}

std::string NoteCanvas::targetTextForWidth(command::Target target, float width, int fontSize){
	const std::string fullTargetName{commands::targetFullName(target)};
	if(doesTextFitWidth(fullTargetName, width, fontSize)) return fullTargetName;


	return commands::targetCompactName(target);
}

void NoteCanvas::drawCenteredTextLine(const Rectangle &columnBounds, float centerY, const std::string &text, Color textColor){
	
	if(text.empty()) return;

	const float availableWidth{std::max(.0f, columnBounds.width - (commands::HorizontalPadding * 2.0f))};
	if(availableWidth <= .0f) return;


	const auto textSize{
		MeasureTextEx(GetFontDefault(), text.c_str(), static_cast<float>(commands::TextFontSize), 1.0f)
	};

	const float startX{
		columnBounds.x + commands::HorizontalPadding + ((availableWidth - textSize.x) * .5f)
	};
	const float startY{centerY - (textSize.y * .5f)};


	DrawTextEx(
		GetFontDefault(),
		text.c_str(),
		Vector2{startX, startY},
		static_cast<float>(commands::TextFontSize),
		1.0f,
		textColor
	);

}

NoteCanvas::BigNote NoteCanvas::createCommandBigNote(
	const command::Command &commandData,
	float width,
	int fontSize
){
    return std::visit([width, fontSize](const auto &command)->BigNote{
        using Type = std::decay_t<decltype(command)>;

        if constexpr(std::is_same_v<Type, command::Tempo>){
            return BigNote{
                .baseColor{commands::TempoColor},
                .iconIndex{commands::TempoIconIndex},
                .firstTextLine{commands::TempoText},
                .secondTextLine{TextFormat(commands::UnsignedIntegerFormatText, command.tempo)},
                .thirdTextLine{commands::EmptyText}
            };
        }else if constexpr(std::is_same_v<Type, command::Volume>){
            return BigNote{
                .baseColor{commands::VolumeColor},
                .iconIndex{commands::VolumeIconIndex},
                .firstTextLine{commands::VolumeText},
                .secondTextLine{TextFormat(commands::UnsignedIntegerFormatText, command.volume)},
                .thirdTextLine{targetTextForWidth(command.target, width, fontSize)}
            };
        // }else if constexpr(std::is_same_v<Type, command::Articulation>){
        //     return BigNote{
        //         .baseColor{commands::ArticulationColor},
        //         .iconIndex{commands::articulationIconIndex(command.articulation)},
        //         .firstTextLine{commands::ArticulationText},
        //         .secondTextLine{commands::articulationName(command.articulation)},
        //         .thirdTextLine{targetTextForWidth(command.target, width, fontSize)}
        //     };
        }else{
			return BigNote{
                .baseColor{commands::ArticulationColor},
                .iconIndex{commands::articulationIconIndex(command.articulation)},
                .firstTextLine{commands::ArticulationText},
                .secondTextLine{commands::articulationName(command.articulation)},
                .thirdTextLine{targetTextForWidth(command.target, width, fontSize)}
            };
        }
    }, commandData);


}