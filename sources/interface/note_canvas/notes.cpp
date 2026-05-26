#include "note_canvas.hpp"

#include <algorithm>
#include <string>
#include <variant>

#include <raygui.h>

#include "debug_utilities.hpp"

#include "constants.hpp"
#include "utilities/project_utilities.hpp"

#include <magic_enum/magic_enum.hpp>

using namespace interface;
namespace canvas_constants = constants::interface_layout::note_canvas;

void NoteCanvas::drawNotes(program_states::InterfaceContext &context){
	auto &noteCanvasState{context.interface.noteCanvas};
	noteCanvasState.cursorPosition = std::nullopt;

	const auto projectData{utilities::projectDataWithPagesFrom(context.system)};
	if(!projectData) return;

	const int currentPageIndex{utilities::currentPageIndexFrom(*projectData, context.system.project.currentPage)};
	const auto &currentPage{projectData->pages[currentPageIndex]};
	const int currentPageNoteCount{utilities::currentPageNoteCountFrom(*projectData, context.system.project.currentPage)};


	const Vector2 mouseCursorPositionInWorld{
		GetScreenToWorld2D(GetMousePosition(), context.system.noteCanvas.gridCamera)
	};
	const Rectangle expandedGridArea{
		noteCanvasState.gridArea.x,
		.0f,
		noteCanvasState.gridArea.width,
		context.layout.bounds.noteCanvas.groupBox.height
	};
	const bool isMouseCursorInsideGridArea{
		CheckCollisionPointRec(mouseCursorPositionInWorld, expandedGridArea)
	};

	const int selectedChannelListViewIndex{context.interface.sidebar.selectedChannelListViewIndex};
	const auto selectedInstrumentChannelIndexValue{utilities::selectedInstrumentChannelIndex(selectedChannelListViewIndex)};
	const bool isAllOrSystemChannelSelection{
		selectedChannelListViewIndex == canvas_constants::notes::AllChannelsListViewIndex
	 || selectedChannelListViewIndex == canvas_constants::notes::SystemChannelListViewIndex
	};
	const bool hasSpecificChannelSelection{selectedInstrumentChannelIndexValue.has_value()};
	const bool shouldShowLabels{isAllOrSystemChannelSelection};
	// DEBUG_PRINT_IF_CHANGED("mouseCursorPositionInWorld: {},{}", mouseCursorPositionInWorld.x, mouseCursorPositionInWorld.y);

	if(isMouseCursorInsideGridArea){
		const float mousePositionRelativeToGridAreaX{
			mouseCursorPositionInWorld.x - noteCanvasState.gridArea.x
		};
		const float mousePositionRelativeToGridAreaY{
			(mouseCursorPositionInWorld.y - noteCanvasState.gridArea.y) + noteCanvasState.verticalScrollInPixels
		};

		const int hoveredNoteIndex{
			static_cast<int>(std::floor(mousePositionRelativeToGridAreaX / noteCanvasState.columnWidth))
		};
		const int hoveredRowIndex{
			static_cast<int>(std::floor(mousePositionRelativeToGridAreaY / noteCanvasState.rowHeightInPixels))
		};


		if(hoveredNoteIndex >= 0
		&& hoveredNoteIndex < constants::project_data::MaximumNotePerPage
		&& hoveredRowIndex >= 0
		&& hoveredRowIndex < canvas_constants::NumberOfRow
		){
			const int hoveredSemitoneFromC0{
				canvas_constants::FirstNoteOffsetFromC0 + (canvas_constants::NumberOfRow - 1) - hoveredRowIndex
			};
			const auto hoveredNote{static_cast<music_data::Note>(hoveredSemitoneFromC0)};
			auto isMatchingCellNote{[&](const auto &cell){
				return cell.has_value()
					&& std::holds_alternative<music_data::Note>(cell.value())
					&& std::get<music_data::Note>(cell.value()) == hoveredNote;
			}};

			const bool isHoveringExistingNote{
				isAllOrSystemChannelSelection
					? std::any_of(
						currentPage.instrumentChannels.begin(),
						currentPage.instrumentChannels.end(),
						[&](const auto &instrumentChannel){
							return isMatchingCellNote(instrumentChannel[hoveredNoteIndex]);
						}
					)
					: (hasSpecificChannelSelection
						? isMatchingCellNote(currentPage.instrumentChannels[static_cast<size_t>(selectedInstrumentChannelIndexValue.value())][hoveredNoteIndex])
						: false
					)
			};

			noteCanvasState.cursorPosition = program_states::Interface::NoteCanvas::CursorPosition{
				.noteIndex = hoveredNoteIndex,
				.note = hoveredNote,
				.isHoveringNote = isHoveringExistingNote
			};
		}

	}

	auto drawNotesIn{[&](size_t channelIndex){
		const bool isSpecificSelectedChannel{
			hasSpecificChannelSelection
		 && channelIndex == static_cast<size_t>(selectedInstrumentChannelIndexValue.value())
		};

		const Color baseColor{canvas_constants::notes::ChannelNoteColors[channelIndex]};

		const float noteAlpha{
			hasSpecificChannelSelection
				? (isSpecificSelectedChannel ? canvas_constants::notes::SelectedChannelAlpha : canvas_constants::notes::UnselectedChannelAlpha)
				: canvas_constants::notes::AllChannelsAlpha
		};
		const Color noteColor{Fade(baseColor, noteAlpha)};

		music_data::Instrument activeInstrument{instrumentAtPageStart(*projectData, channelIndex, currentPageIndex)};
		bool shouldLabelFirstNoteInPage{shouldShowLabels};
		bool shouldLabelNextNoteAfterInstrumentChange{false};

		DEBUG_PRINT_IF_CHANGED("currentPageNoteCount: {}", currentPageNoteCount);


		for(int noteColumnIndex{0}; noteColumnIndex < currentPageNoteCount; noteColumnIndex++){
			const auto &cell{currentPage.instrumentChannels[channelIndex][noteColumnIndex]};

			if(!cell.has_value()) continue;

			// DEBUG_PRINT_IF_CHANGED(
			// 	"channelIndex:{} noteColumnIndex:{} -> {}",
			// 	channelIndex,
			// 	noteColumnIndex,
			// 	std::holds_alternative<music_data::Instrument>(cell.value())
			// 		? fmt::format("instrument {}", static_cast<int>(std::get<music_data::Instrument>(cell.value())))
			// 		: fmt::format("note {}", static_cast<int>(std::get<music_data::Note>(cell.value())))
			// );

			if(std::holds_alternative<music_data::Instrument>(cell.value())){
				activeInstrument = std::get<music_data::Instrument>(cell.value());
				shouldLabelNextNoteAfterInstrumentChange = shouldShowLabels;

				if(isSpecificSelectedChannel){
					const float topY{rowEdgeY(context, 0)};
					const float bottomY{rowEdgeY(context, canvas_constants::NumberOfRow)};
					const Rectangle bounds{
						noteCanvasState.gridArea.x + (static_cast<float>(noteColumnIndex) * noteCanvasState.columnWidth),
						topY,
						noteCanvasState.columnWidth,
						bottomY - topY
					};

					const auto instrumentBigNote{
						NoteCanvas::createInstrumentBigNote(baseColor, activeInstrument)
					};
					NoteCanvas::drawBigNote(context, bounds, instrumentBigNote, noteAlpha, true);
				}

				continue;
			}

			const int rowIndex{
				canvas_constants::FirstNoteOffsetFromC0 + (canvas_constants::NumberOfRow - 1) - static_cast<int>(std::get<music_data::Note>(cell.value()))
			};
			if(rowIndex < 0 || rowIndex >= canvas_constants::NumberOfRow) continue;
			

			const float topY{rowEdgeY(context, rowIndex)};
			const float bottomY{rowEdgeY(context, rowIndex + 1)};
			const Rectangle noteBounds{
				noteCanvasState.gridArea.x + (static_cast<float>(noteColumnIndex) * noteCanvasState.columnWidth),
				topY,
				noteCanvasState.columnWidth,
				bottomY - topY
			};

			DrawRectangleRec(noteBounds, noteColor);
			DrawRectangleLinesEx(noteBounds, canvas_constants::notes::BorderThicknessInPixels, Fade(canvas_constants::notes::NoteBorderColor, noteAlpha));

			if(shouldShowLabels && (shouldLabelFirstNoteInPage || shouldLabelNextNoteAfterInstrumentChange)){
				const Rectangle iconBounds{noteBounds.x, noteBounds.y - canvas_constants::IconSize - canvas_constants::InstrumentIconPadding, canvas_constants::IconSize, canvas_constants::IconSize};
				const int previousTextColor{GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)};
				const std::string instrumentIconText{constants::instruments::instrumentIconMarkupText(activeInstrument)};

				GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(noteColor));
				GuiLabel(iconBounds, instrumentIconText.c_str());
				GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, previousTextColor);

				shouldLabelFirstNoteInPage = false;
				shouldLabelNextNoteAfterInstrumentChange = false;
			}
		}
	}};

	for(size_t instrumentChannelIndex{currentPage.instrumentChannels.size()}; instrumentChannelIndex --> 0;){
		if(hasSpecificChannelSelection 
		&& instrumentChannelIndex == selectedInstrumentChannelIndexValue.value()){
			continue;
		}
		drawNotesIn(instrumentChannelIndex);
	}
	if(hasSpecificChannelSelection) drawNotesIn(selectedInstrumentChannelIndexValue.value());

	// DEBUG_PRINT_IF_CHANGED(
	// 	"selectedChannelIndex: {}, hasSpecificChannelSelection: {}",
	// 	selectedChannelIndex, hasSpecificChannelSelection
	// );

	// if(noteCanvasState.cursorPosition){
	// 	DEBUG_PRINT_IF_CHANGED(
	// 		"Position: {}, Note: {}, Hover: {}",
	// 		noteCanvasState.cursorPosition.value().noteIndex,
	// 		magic_enum::enum_name<music_data::Note>(noteCanvasState.cursorPosition.value().note),
	// 		noteCanvasState.cursorPosition.value().isHoveringNote
	// 	);
	// }


}

NoteCanvas::BigNote NoteCanvas::createInstrumentBigNote(Color baseColor, music_data::Instrument instrument){
	std::string instrumentName{std::string{magic_enum::enum_name<music_data::Instrument>(instrument)}};
	std::replace(instrumentName.begin(), instrumentName.end(), '_', ' ');

	return BigNote{
		.baseColor{baseColor},
		.iconIndex{constants::instruments::InstrumentIconMappings[instrument]},
		.firstTextLine{instrumentName},
		.secondTextLine{canvas_constants::commands::EmptyText},
		.thirdTextLine{canvas_constants::commands::EmptyText},
		.shouldDrawFirstTextLineVertically{true}
	};
}