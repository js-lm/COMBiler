#include "note_canvas.hpp"

#include <algorithm>
#include <string>
#include <variant>

#include <raygui.h>

#include "debug_utilities.hpp"

#include "constants.hpp"

using namespace interface;
using namespace constants::interface_layout::note_canvas;

void NoteCanvas::drawNotes(program_states::Context &context){
	const auto projectData{context.system.project.data.lock()};

	if(!projectData || projectData->pages.empty()) return;

	const int currentPageIndex{context.system.project.currentPage - 1};
	const auto &currentPage{projectData->pages[currentPageIndex]};
	const int currentPageNoteCount{
		std::clamp(
			currentPage.noteInThisPage.value_or(projectData->metadata.noteInThisPage),
			constants::project_data::MinimumNotePerPage,
			constants::project_data::MaximumNotePerPage
		)
	};
	auto &noteCanvasState{context.interface.noteCanvas};

	const int selectedChannelIndex{context.interface.sidebar.selectedChannelListViewIndex};

	const bool shouldShowLabels{
		selectedChannelIndex == notes::AllChannelsListViewIndex
	 || selectedChannelIndex == notes::SystemChannelListViewIndex
	};
	const bool hasSpecificChannelSelection{
		selectedChannelIndex > notes::AllChannelsListViewIndex
	 && selectedChannelIndex < notes::SystemChannelListViewIndex
	};

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

	DEBUG_PRINT_IF_CHANGED(
		"selectedChannelIndex: {}, hasSpecificChannelSelection: {}",
		selectedChannelIndex, hasSpecificChannelSelection
	);

}