#include "note_canvas.hpp"

#include <algorithm>
#include <string>
#include <variant>

#include <raygui.h>

#include "debug_utilities.hpp"

#include "constants.hpp"

#include <magic_enum/magic_enum.hpp>

using namespace interface;
using namespace constants::interface_layout::note_canvas;

void NoteCanvas::drawNotes(program_states::InterfaceContext &context){
	auto &noteCanvasState{context.interface.noteCanvas};
	noteCanvasState.cursorPosition = std::nullopt;

	const auto projectDataSlot{context.system.project.data.lock()};

	if(!projectDataSlot || !projectDataSlot->data || projectDataSlot->data->pages.empty()) return;

	const auto &projectData{projectDataSlot->data};

	const int currentPageIndex{context.system.project.currentPage - 1};
	const auto &currentPage{projectData->pages[currentPageIndex]};
	const int currentPageNoteCount{
		std::clamp(
			currentPage.noteInThisPage.value_or(projectData->metadata.noteInThisPage),
			constants::project_data::MinimumNotePerPage,
			constants::project_data::MaximumNotePerPage
		)
	};


	const Vector2 mouseCursorPositionInWorld{
		GetScreenToWorld2D(GetMousePosition(), context.system.noteCanvas.gridCamera)
	};
	const bool isMouseCursorInsideGridArea{
		CheckCollisionPointRec(mouseCursorPositionInWorld, noteCanvasState.gridArea)
	};

	const int selectedChannelIndex{context.interface.sidebar.selectedChannelListViewIndex};
	const bool isAllOrSystemChannelSelection{
		selectedChannelIndex == notes::AllChannelsListViewIndex
	 || selectedChannelIndex == notes::SystemChannelListViewIndex
	};
	const bool hasSpecificChannelSelection{
		selectedChannelIndex >= 1
	 && selectedChannelIndex <= static_cast<int>(currentPage.instrumentChannels.size())
	};
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
		&& hoveredRowIndex < NumberOfRow
		){
			const int hoveredSemitoneFromC0{
				FirstNoteOffsetFromC0 + (NumberOfRow - 1) - hoveredRowIndex
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
						? isMatchingCellNote(currentPage.instrumentChannels[static_cast<size_t>(selectedChannelIndex - 1)][hoveredNoteIndex])
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
		 && channelIndex == selectedChannelIndex - 1
		};

		const Color baseColor{notes::ChannelNoteColors[channelIndex]};

		const float noteAlpha{
			hasSpecificChannelSelection
				? (isSpecificSelectedChannel ? notes::SelectedChannelAlpha : notes::UnselectedChannelAlpha)
				: notes::AllChannelsAlpha
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
					const float bottomY{rowEdgeY(context, NumberOfRow)};
					const Rectangle bounds{
						noteCanvasState.gridArea.x + (static_cast<float>(noteColumnIndex) * noteCanvasState.columnWidth),
						topY,
						noteCanvasState.columnWidth,
						bottomY - topY
					};

					// const Color darkerBaseColor{
					// 	static_cast<unsigned char>(static_cast<float>(baseColor.r) * notes::InstrumentIndexColorDarkeningFactor),
					// 	static_cast<unsigned char>(static_cast<float>(baseColor.g) * notes::InstrumentIndexColorDarkeningFactor),
					// 	static_cast<unsigned char>(static_cast<float>(baseColor.b) * notes::InstrumentIndexColorDarkeningFactor),
					// 	baseColor.a
					// };
					const Color fillColor{Fade(ColorBrightness(baseColor, -notes::InstrumentIndexColorDarkeningFactor), noteAlpha)};

					DrawRectangleRec(bounds, fillColor);
					DrawRectangleLinesEx(bounds, notes::BorderThicknessInPixels, Fade(notes::NoteBorderColor, noteAlpha));

					// const int previousTextColor{GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)};
					// const int previousTextAlignment{GuiGetStyle(DEFAULT, TEXT_ALIGNMENT)};

					// DEBUG_PRINT_TIMED(1000, "Before: previousTextColor: {}, previousTextAlignment:{}",
					// 	previousTextColor, previousTextAlignment
					// );

					// GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(Fade(notes::InstrumentIndexIconColor, noteAlpha)));
					// GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

					// DEBUG_PRINT_TIMED(1000, "After: TEXT_COLOR_NORMAL: {}, TEXT_ALIGNMENT:{}",
					// 	GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL), GuiGetStyle(DEFAULT, TEXT_ALIGNMENT)
					// );
					
					// GuiLabel(bounds, TextFormat("#%d#", constants::InstrumentIconMappings[activeInstrument]));

					// GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, previousTextColor);
					// GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, previousTextAlignment);


					// DEBUG_PRINT_TIMED(1000, "Restored: TEXT_COLOR_NORMAL: {}, TEXT_ALIGNMENT:{}",
					// 	GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL), GuiGetStyle(DEFAULT, TEXT_ALIGNMENT)
					// );

					GuiDrawIcon(
						constants::InstrumentIconMappings[activeInstrument], 
						bounds.x + (bounds.width - IconSize) * .5f, 
						bounds.y + (bounds.height - IconSize) * .5f, 
						1, 
						Fade(notes::InstrumentIndexIconColor, noteAlpha)
					);
				}

				continue;
			}

			const int rowIndex{
				FirstNoteOffsetFromC0 + (NumberOfRow - 1) - static_cast<int>(std::get<music_data::Note>(cell.value()))
			};
			if(rowIndex < 0 || rowIndex >= NumberOfRow) continue;
			

			const float topY{rowEdgeY(context, rowIndex)};
			const float bottomY{rowEdgeY(context, rowIndex + 1)};
			const Rectangle noteBounds{
				noteCanvasState.gridArea.x + (static_cast<float>(noteColumnIndex) * noteCanvasState.columnWidth),
				topY,
				noteCanvasState.columnWidth,
				bottomY - topY
			};

			DrawRectangleRec(noteBounds, noteColor);
			DrawRectangleLinesEx(noteBounds, notes::BorderThicknessInPixels, Fade(notes::NoteBorderColor, noteAlpha));

			if(shouldShowLabels && (shouldLabelFirstNoteInPage || shouldLabelNextNoteAfterInstrumentChange)){
				const Rectangle iconBounds{noteBounds.x, noteBounds.y - IconSize - InstrumentIconPadding, IconSize, IconSize};
				const int previousTextColor{GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)};

				GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(noteColor));
				GuiLabel(iconBounds, TextFormat("#%d#", constants::InstrumentIconMappings[activeInstrument]));
				GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, previousTextColor);

				shouldLabelFirstNoteInPage = false;
				shouldLabelNextNoteAfterInstrumentChange = false;
			}
		}
	}};

	for(size_t instrumentChannelIndex{currentPage.instrumentChannels.size()}; instrumentChannelIndex --> 0;){
		if(hasSpecificChannelSelection 
		&& instrumentChannelIndex == selectedChannelIndex - 1){
			continue;
		}
		drawNotesIn(instrumentChannelIndex);
	}
	if(hasSpecificChannelSelection) drawNotesIn(selectedChannelIndex - 1);

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