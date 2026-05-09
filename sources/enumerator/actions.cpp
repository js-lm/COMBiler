#include "enumerator.hpp"

std::vector<Enumerator::EncodedRow> Enumerator::compileProjectData(const program_states::ProjectData &projectData){

    std::vector<EncodedRow> compiledData{};

    int totalRows{0};
    for(const auto &page : projectData.pages){
        totalRows += page.noteInThisPage.value_or(projectData.metadata.notePerPage);
    }

    compiledData.reserve(totalRows);

    for(int pageIndex{0}; pageIndex < projectData.pages.size(); pageIndex++){

        const auto  &page{projectData.pages[pageIndex]};
        
        int notesInPage{page.noteInThisPage.value_or(projectData.metadata.notePerPage)};

        for(int noteIndex{0}; noteIndex < notesInPage; noteIndex++){
            EncodedRow rowData{};

            for(int channelIndex{0}; channelIndex < constants::project_data::NumberOfInstrumentChannels; channelIndex++){
                EncodedInstruction instrumentData{encodeInstrumentData(page.instrumentChannels[channelIndex][noteIndex])};
                
                int baseIndex{channelIndex * constants::enumerator::NumberOfDigitPerInstruction};
                rowData[baseIndex]     = instrumentData[0];
                rowData[baseIndex + 1] = instrumentData[1];
                rowData[baseIndex + 2] = instrumentData[2];
            }

            EncodedInstruction systemData{encodeCommandData(page.commandChannel[noteIndex], projectData.commandPalette)};
            rowData[constants::enumerator::SystemCommandStartIndex] = systemData[0];
            rowData[constants::enumerator::SystemCommandStartIndex + 1] = systemData[1];
            rowData[constants::enumerator::SystemCommandStartIndex + 2] = systemData[2];

            int checksumSum{0};
            for(int checksumIndex{0}; checksumIndex < constants::enumerator::ChecksumIndex; checksumIndex++){
                checksumSum += static_cast<int>(rowData[checksumIndex]);
            }

            int checksumValue{(constants::enumerator::EncodingBase 
                             - (checksumSum % constants::enumerator::EncodingBase)
                            ) % constants::enumerator::EncodingBase
            };
            rowData[constants::enumerator::ChecksumIndex] = static_cast<units::enumerator::Color>(checksumValue);

            compiledData.push_back(rowData);

        }
    }

    return compiledData;
}

Enumerator::EncodedInstruction Enumerator::encodeInstrumentData(const std::optional<music_data::InstrumentChannelData> &channelData){
    if(!channelData.has_value()){
        return {
            static_cast<units::enumerator::Color>(0), 
            static_cast<units::enumerator::Color>(0), 
            static_cast<units::enumerator::Color>(0)
        };
    }

    return std::visit([](const auto &data)->EncodedInstruction{
        using Type = std::decay_t<decltype(data)>;

        if constexpr(std::is_same_v<Type, music_data::Note>){
            int noteValue{static_cast<int>(data)};
            
            
            // int operationCode{noteValue / constants::enumerator::EncodingBaseSquared};
            // int remainder{noteValue % constants::enumerator::EncodingBaseSquared};
            // int immediateDigit1{remainder / constants::enumerator::EncodingBase};
            // int immediateDigit2{remainder % constants::enumerator::EncodingBase};


            // return {
            //     static_cast<units::enumerator::Color>(operationCode),
            //     static_cast<units::enumerator::Color>(immediateDigit1),
            //     static_cast<units::enumerator::Color>(immediateDigit2)
            // };
            
            return splitToBase5(noteValue - constants::interface_layout::note_canvas::FirstNoteOffsetFromC0);

        }else if constexpr(std::is_same_v<Type, music_data::Instrument>){

            int instrumentValue{static_cast<int>(data)};
            
            
            // int operationCode{
            //     instrumentValue / constants::enumerator::EncodingBaseSquared
            //   + constants::enumerator::SetInstrumentOpcodeOffset
            // };
            // int remainder{instrumentValue % constants::enumerator::EncodingBaseSquared};
            // int immediateDigit1{remainder / constants::enumerator::EncodingBase};
            // int immediateDigit2{remainder % constants::enumerator::EncodingBase};

            // return {
            //     static_cast<units::enumerator::Color>(operationCode),
            //     static_cast<units::enumerator::Color>(immediateDigit1),
            //     static_cast<units::enumerator::Color>(immediateDigit2)
            // };
            
            return splitToBase5(instrumentValue, constants::enumerator::SetInstrumentOpcodeOffset);
        }


        // return {
        //     static_cast<units::enumerator::Color>(0), 
        //     static_cast<units::enumerator::Color>(0), 
        //     static_cast<units::enumerator::Color>(0)
        // };

    }, channelData.value());


}

Enumerator::EncodedInstruction Enumerator::encodeCommandData(
    const std::optional<command::CommandToken>  &commandToken, 
    const command::CommandPalette               &commandPalette
){
    if(!commandToken.has_value()){

        return {
            static_cast<units::enumerator::Color>(0), 
            static_cast<units::enumerator::Color>(0), 
            static_cast<units::enumerator::Color>(0)
        };
    }

    std::optional<command::Command> rawCommand{};


    std::visit([&](const auto &data){
        using Type = std::decay_t<decltype(data)>;

        if constexpr(std::is_same_v<Type, command::Command>){
            rawCommand = data;
        }else if constexpr(std::is_same_v<Type, command::ConstantIndex>){

            auto constantsList{commandPalette.getList()};
            int constantIndex{static_cast<int>(data)};
            
            if(constantIndex >= 0 && constantIndex < constantsList.size()){
                rawCommand = constantsList[constantIndex].second;
            }
        }

    }, commandToken.value());

    if(!rawCommand.has_value()){
        return {
            static_cast<units::enumerator::Color>(0), 
            static_cast<units::enumerator::Color>(0), 
            static_cast<units::enumerator::Color>(0)
        };
    }


    return std::visit([](const auto &data)->EncodedInstruction{

        using Type = std::decay_t<decltype(data)>;


        if constexpr(std::is_same_v<Type, command::Tempo>){
            int tempoValue{
                data.tempo / constants::enumerator::TempoMultiplier 
              - constants::enumerator::TempoOffset
            };
            
            return splitToBase5(tempoValue);

        }else if constexpr(std::is_same_v<Type, command::Volume>){
            
            int volumeValue{
                data.volume - constants::enumerator::VolumeOffset
            };
            

            return {
                static_cast<units::enumerator::Color>((volumeValue / constants::enumerator::EncodingBase) + constants::enumerator::SetVolumeOpcodeOffset),
                static_cast<units::enumerator::Color>(data.target),
                static_cast<units::enumerator::Color>(volumeValue % constants::enumerator::EncodingBase)
            };


        }else if constexpr(std::is_same_v<Type, command::Articulation>){
            int operationCode{constants::enumerator::SetArticulationOpcode};
            int immediateDigit1{static_cast<int>(data.target)};
            int immediateDigit2{static_cast<int>(data.articulation)};

            return {
                static_cast<units::enumerator::Color>(operationCode),
                static_cast<units::enumerator::Color>(immediateDigit1),
                static_cast<units::enumerator::Color>(immediateDigit2)
            };
        }

    }, rawCommand.value());
}