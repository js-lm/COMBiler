#include "note_canvas.hpp"

#include <algorithm>
#include <cmath>
#include <string>
#include <variant>

#include "constants.hpp"

#include "debug_utilities.hpp"

#include "utilities/project_utilities.hpp"

using namespace interface;
namespace canvas_constants = constants::interface_layout::note_canvas;

void NoteCanvas::drawPasteGhost(program_states::InterfaceContext &context){
    const auto &clipboardState{context.interface.clipboard};

    if(!clipboardState.isPasteModeEnabled || !clipboardState.hasCopiedSelection) return;
    if(!clipboardState.hasPasteAnchor) return;

    const int destinationTopLeftColumnIndex{
        clipboardState.pasteAnchorNoteIndex - (clipboardState.copiedWidthInCells / 2)
    };
    const int destinationCenterRowIndex{clipboardState.pasteAnchorCenterRowIndex};
    const int destinationTopLeftRowIndex{destinationCenterRowIndex - (clipboardState.copiedHeightInCells / 2)};
    const int transposeSemitoneOffset{clipboardState.copiedCenterRowIndex - destinationCenterRowIndex};

    const auto selectedInstrumentChannelIndexValue{
        utilities::selectedInstrumentChannelIndex(context.interface.sidebar.selectedChannelListViewIndex)
    };
    int destinationInstrumentChannelIndex{clipboardState.copiedInstrumentChannelIndex};
    if(!clipboardState.copiedFromAllChannels && selectedInstrumentChannelIndexValue.has_value()){
        destinationInstrumentChannelIndex = selectedInstrumentChannelIndexValue.value();
    }

    DEBUG_PRINT_IF_CHANGED(
        "destinationInstrumentChannelIndex:{} copiedInstrumentChannelIndex:{} copiedFromAllChannels:{}",
        destinationInstrumentChannelIndex,
        clipboardState.copiedInstrumentChannelIndex,
        clipboardState.copiedFromAllChannels
    );

    auto drawGhostNoteCell{[&](int destinationColumnIndex, music_data::Note note, Color color, int transposeOffset){
        if(destinationColumnIndex < 0 || destinationColumnIndex >= constants::project_data::MaximumNotePerPage) return;

        const int rowIndex{
            canvas_constants::FirstNoteOffsetFromC0 + (canvas_constants::NumberOfRow - 1) - (static_cast<int>(note) + transposeOffset)
        };
        if(rowIndex < 0 || rowIndex >= canvas_constants::NumberOfRow) return;

        const float topY{rowEdgeY(context, rowIndex)};
        const float bottomY{rowEdgeY(context, rowIndex + 1)};

        const Rectangle noteBounds{
            context.interface.noteCanvas.gridArea.x + (static_cast<float>(destinationColumnIndex) * context.interface.noteCanvas.columnWidth),
            topY,
            context.interface.noteCanvas.columnWidth,
            bottomY - topY
        };

        DrawRectangleRec(noteBounds, Fade(color, canvas_constants::paste_ghost::NoteFillAlpha));
        DrawRectangleLinesEx(noteBounds, canvas_constants::notes::BorderThicknessInPixels, Fade(WHITE, canvas_constants::paste_ghost::NoteBorderAlpha));
        
    }};

    auto drawGhostInstrumentCell{[&](int destinationColumnIndex, music_data::Instrument instrument, Color color){

        if(destinationColumnIndex < 0 || destinationColumnIndex >= constants::project_data::MaximumNotePerPage) return;

        const float topY{rowEdgeY(context, 0)};
        const float bottomY{rowEdgeY(context, canvas_constants::NumberOfRow)};

        const Rectangle bounds{
            context.interface.noteCanvas.gridArea.x + (static_cast<float>(destinationColumnIndex) * context.interface.noteCanvas.columnWidth),
            topY,
            context.interface.noteCanvas.columnWidth,
            bottomY - topY
        };

        const auto instrumentBigNote{createInstrumentBigNote(color, instrument)};
        drawBigNote(context, bounds, instrumentBigNote, canvas_constants::paste_ghost::BigNoteAlpha, true);

    }};

    auto drawGhostCommandCell{[&](int destinationColumnIndex, const command::CommandToken &commandToken){
        if(destinationColumnIndex < 0 || destinationColumnIndex >= constants::project_data::MaximumNotePerPage) return;

        auto projectDataForGhost{context.system.project.data.lock()};
        if(!projectDataForGhost) return;
        auto commandData{commandFromToken(commandToken, *(projectDataForGhost->data))};
        if(!commandData.has_value()) return;

        if(!clipboardState.copiedFromAllChannels){


            if(destinationInstrumentChannelIndex >= 0){

                const auto targetForChannel{
                    static_cast<command::Target>(destinationInstrumentChannelIndex + 1)
                };


                commandData = std::visit([targetForChannel](const auto &commandVariant)->command::Command{
                    using Type = std::decay_t<decltype(commandVariant)>;

                    if constexpr(std::is_same_v<Type, command::Tempo>){
                        return commandVariant;
                    }else if constexpr(std::is_same_v<Type, command::Volume>){
                        return command::Volume{.volume{commandVariant.volume}, .target{targetForChannel}};
                    }else{
                        return command::Articulation{.articulation{commandVariant.articulation}, .target{targetForChannel}};
                    }
                }, commandData.value());

            }

        }

        const float topY{rowEdgeY(context, 0)};
        const float bottomY{rowEdgeY(context, canvas_constants::NumberOfRow)};

        const Rectangle columnBounds{
            context.interface.noteCanvas.gridArea.x + (static_cast<float>(destinationColumnIndex) * context.interface.noteCanvas.columnWidth),
            topY,
            context.interface.noteCanvas.columnWidth,
            bottomY - topY
        };

        const float textWidth{std::max(.0f, columnBounds.width - (canvas_constants::commands::HorizontalPadding * 2.0f))};
        auto commandBigNote{createCommandBigNote(commandData.value(), textWidth, canvas_constants::commands::TextFontSize)};

		if(std::holds_alternative<command::ConstantIndex>(commandToken)){
            
			if(auto projectData{context.system.project.data.lock()}){

				const auto constantIndex{std::get<command::ConstantIndex>(commandToken)};
				const auto &commandList{projectData->data->commandPalette.getList()};

				commandBigNote.constantName = commandList[constantIndex].first;
			}
		}

        drawBigNote(context, columnBounds, commandBigNote, canvas_constants::paste_ghost::BigNoteAlpha, false);


    }};

    auto drawSingleChannelGhostAt{[&](int sourceColumnOffset, int destinationColumnIndex, int channelIndex){
        const auto &cell{clipboardState.instrumentChannels[channelIndex][sourceColumnOffset]};
        if(!cell.has_value()) return;

        const Color channelColor{canvas_constants::notes::ChannelNoteColors[channelIndex]};
        const int effectiveTransposeOffset{
            clipboardState.isChannelDrumSet[channelIndex][sourceColumnOffset] ? 0 : transposeSemitoneOffset
        };

        if(std::holds_alternative<music_data::Note>(cell.value())){

            drawGhostNoteCell(destinationColumnIndex, std::get<music_data::Note>(cell.value()), channelColor, effectiveTransposeOffset);

        // }else{
        //     drawGhostInstrumentCell(destinationColumnIndex, std::get<music_data::Instrument>(cell.value()), channelColor);
        }
    }};

    for(int sourceColumnOffset{0}; sourceColumnOffset < clipboardState.copiedWidthInCells; sourceColumnOffset++){
        const int destinationColumnIndex{destinationTopLeftColumnIndex + sourceColumnOffset};

        if(clipboardState.copiedFromAllChannels){
            for(int instrumentChannelIndex{0}; instrumentChannelIndex < constants::project_data::NumberOfInstrumentChannels; instrumentChannelIndex++){
                drawSingleChannelGhostAt(sourceColumnOffset, destinationColumnIndex, instrumentChannelIndex);
            }
        }else if(clipboardState.copiedInstrumentChannelIndex >= 0){
            const int sourceInstrumentChannelIndex{clipboardState.copiedInstrumentChannelIndex};
            const auto &cell{clipboardState.instrumentChannels[sourceInstrumentChannelIndex][sourceColumnOffset]};

            if(cell.has_value()){

                const Color channelColor{canvas_constants::notes::ChannelNoteColors[destinationInstrumentChannelIndex]};
                const int effectiveTransposeOffset{
                    clipboardState.isChannelDrumSet[sourceInstrumentChannelIndex][sourceColumnOffset] ? 0 : transposeSemitoneOffset
                };

                if(std::holds_alternative<music_data::Note>(cell.value())){
                    drawGhostNoteCell(destinationColumnIndex, std::get<music_data::Note>(cell.value()), channelColor, effectiveTransposeOffset);
                // }else{
                //     drawGhostInstrumentCell(destinationColumnIndex, std::get<music_data::Instrument>(cell.value()), channelColor);

                }
            }

        }

        if(context.interface.sidebar.isShowCommandsEnabled){
            const auto &commandToken{clipboardState.commandChannel[sourceColumnOffset]};

            if(commandToken.has_value()){

                drawGhostCommandCell(destinationColumnIndex, commandToken.value());

            }
        }

    }

    const float rightPositionX{
        context.interface.noteCanvas.gridArea.x
      + (static_cast<float>(destinationTopLeftColumnIndex + clipboardState.copiedWidthInCells) * context.interface.noteCanvas.columnWidth)
    };
    const float bottomPositionY{rowEdgeY(context, destinationTopLeftRowIndex + clipboardState.copiedHeightInCells)};

    const std::string transposeText{TextFormat("%+d", transposeSemitoneOffset)};
    DrawTextEx(
        GetFontDefault(),
        transposeText.c_str(),
        Vector2{
            std::round(rightPositionX + canvas_constants::paste_ghost::TransposeTextOffsetInPixels),
            std::round(bottomPositionY + canvas_constants::paste_ghost::TransposeTextOffsetInPixels)
        },
        static_cast<float>(canvas_constants::paste_ghost::TransposeTextFontSize),
        canvas_constants::paste_ghost::TransposeTextSpacingInPixels,
        canvas_constants::selection::BorderColor
    );


}
