#include "action_center.hpp"

void ActionCenter::commitConstantModification(const program_states::Interface::Prompts::ConstantsManagerDraft &draft){
    beginAction();

    auto &palette{stagedSlot_->data->commandPalette};

    if(const auto &commandTypeOptional{draft.commandType};
        commandTypeOptional
     && commandTypeOptional.value() != constants::prompts::constants_manager::Type::None
    ){
        const auto &commandType{commandTypeOptional.value()};

        std::string baseName{draft.nameTextBoxText};
        std::string name{baseName};

        constexpr int MaximumTextLength{constants::prompts::constants_manager::ConstantNameTextMaximumLength - 1};

        for(int duplicateCounter{0};;){
            auto existingIndex{palette.getIndex(name)};

            if(!existingIndex.has_value() 
            || existingIndex.value() == draft.constantIndex
            ){ 
                break;
            }

            duplicateCounter++;

            std::string suffix{" (" + std::to_string(duplicateCounter) + ")"};
            if(baseName.length() + suffix.length() > MaximumTextLength){
                name = baseName.substr(0, MaximumTextLength - suffix.length()) + suffix;
            }else{
                name = baseName + suffix;
            }

        }

        command::Command commandData{};


        switch(commandType){
        case constants::prompts::constants_manager::Type::Tempo: {
            commandData = command::Tempo{static_cast<units::machine::Tempo>(draft.tempoSliderValue)};

        } break;
        case constants::prompts::constants_manager::Type::Volume: {
            commandData = command::Volume{
                static_cast<units::machine::Volume>(draft.volumeToggleGroupIndex),
                static_cast<command::Target>(draft.targetToggleGroupIndex)
            };
        } break;
        case constants::prompts::constants_manager::Type::Articulation: {
            commandData = command::Articulation{
                static_cast<units::machine::Articulation>(draft.articulationToggleGroupIndex),
                static_cast<command::Target>(draft.targetToggleGroupIndex)
            };
        } break;
        }

        palette.set(draft.constantIndex, name, commandData);


    }else{
        palette.remove(draft.constantIndex);
    }
    
    
    finishAction();

}