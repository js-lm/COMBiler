#include "note_canvas.hpp"

#include <algorithm>
#include <string>
#include <cmath>

#include <raygui.h>

#include "constants.hpp"
#include "utilities/project_utilities.hpp"

using namespace interface;
namespace canvas_constants = constants::interface_layout::note_canvas;

void NoteCanvas::drawCommands(program_states::InterfaceContext &context){

	const auto projectData{utilities::projectDataWithPagesFrom(context.system)};
	if(!projectData) return;

	auto &noteCanvasState{context.interface.noteCanvas};
	const auto &sidebarState{context.interface.sidebar};

	const int selectedChannelListViewIndex{sidebarState.selectedChannelListViewIndex};
	const bool isSystemChannelSelection{
		selectedChannelListViewIndex == canvas_constants::notes::SystemChannelListViewIndex
	};
	const bool shouldShowCommandsEverywhere{sidebarState.isShowCommandsEnabled};
	const bool shouldDrawCommands{isSystemChannelSelection || shouldShowCommandsEverywhere};
	if(!shouldDrawCommands) return;

	const float commandAlpha{
		isSystemChannelSelection
			? canvas_constants::commands::FullVisibilityAlpha
			: canvas_constants::commands::TransparentVisibilityAlpha
	};


	const int currentPageIndex{utilities::currentPageIndexFrom(*projectData, context.system.project.currentPage)};
	const auto &currentPage{projectData->pages[currentPageIndex]};
	const int currentPageNoteCount{utilities::currentPageNoteCountFrom(*projectData, context.system.project.currentPage)};

	const float topY{rowEdgeY(context, 0)};
	const float bottomY{rowEdgeY(context, canvas_constants::NumberOfRow)};
	const float commandHeight{bottomY - topY};


	for(int noteColumnIndex{0}; noteColumnIndex < currentPageNoteCount; noteColumnIndex++){
		
		const auto &commandTokenCell{currentPage.commandChannel[noteColumnIndex]};
		if(!commandTokenCell.has_value()) continue;

		const auto commandData{commandFromToken(commandTokenCell.value(), *projectData)};
		if(!commandData) continue;

		const Rectangle columnBounds{
			noteCanvasState.gridArea.x + (static_cast<float>(noteColumnIndex) * noteCanvasState.columnWidth),
			topY,
			noteCanvasState.columnWidth,
			commandHeight
		};

		const float textWidth{std::max(.0f, columnBounds.width - (canvas_constants::commands::HorizontalPadding * 2.0f))};
		auto commandBigNote{
			NoteCanvas::createCommandBigNote(commandData.value(), textWidth, canvas_constants::commands::TextFontSize)
		};

		if(std::holds_alternative<command::ConstantIndex>(commandTokenCell.value())){
			if(auto projectData{context.system.project.data.lock()}){
				const auto constantIndex{std::get<command::ConstantIndex>(commandTokenCell.value())};
				const auto &commandList{projectData->data->commandPalette.getList()};
				commandBigNote.constantName = commandList[constantIndex].first;
			}
		}

		NoteCanvas::drawBigNote(context, columnBounds, commandBigNote, commandAlpha, false);

	}



}

std::optional<command::Command> NoteCanvas::commandFromToken(const command::CommandToken &token, const program_states::ProjectData &projectData){
	return projectData.commandPalette.get(token);
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
	const std::string fullTargetName{canvas_constants::commands::targetFullName(target)};
	if(doesTextFitWidth(fullTargetName, width, fontSize)) return fullTargetName;


	return canvas_constants::commands::targetCompactName(target);
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
                .baseColor{canvas_constants::commands::TempoColor},
                .iconIndex{canvas_constants::commands::TempoIconIndex},
                .firstTextLine{canvas_constants::commands::TempoText},
                .secondTextLine{TextFormat(canvas_constants::commands::UnsignedIntegerFormatText, command.tempo)},
                .thirdTextLine{canvas_constants::commands::EmptyText}
            };
        }else if constexpr(std::is_same_v<Type, command::Volume>){
            return BigNote{
                .baseColor{canvas_constants::commands::VolumeColor},
                .iconIndex{canvas_constants::commands::VolumeIconIndex},
                .firstTextLine{canvas_constants::commands::VolumeText},
                .secondTextLine{TextFormat(canvas_constants::commands::UnsignedIntegerFormatText, command.volume + 1)},
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
                .baseColor{canvas_constants::commands::ArticulationColor},
                .iconIndex{canvas_constants::commands::articulationIconIndex(command.articulation)},
                .firstTextLine{canvas_constants::commands::ArticulationText},
                .secondTextLine{canvas_constants::commands::articulationName(command.articulation)},
                .thirdTextLine{targetTextForWidth(command.target, width, fontSize)}
            };
        }
    }, commandData);


}