#include "midi_manager.hpp"

#include <string>

#include "constants.hpp"

#include "debug_utilities.hpp"

#include <magic_enum/magic_enum.hpp>

program_states::System::InitializationError MidiManager::initialization(){

    DEBUG_PRINT("IsAudioDeviceReady(): {}", IsAudioDeviceReady());

    InitAudioDevice();

#ifdef PLATFORM_WEB
    const std::string fontDirectory{constants::midi::SoundFontDirectory};
    const std::string targetFontFile{constants::midi::SoundFontFile};

    std::string soundFontPath{fontDirectory + targetFontFile};

    if(!FileExists(soundFontPath.c_str())){
        FilePathList fontDirectoryFiles{LoadDirectoryFilesEx(fontDirectory.c_str(), constants::midi::SoundFontExtension, false)};
        if(fontDirectoryFiles.count > 0){
            soundFontPath = fontDirectoryFiles.paths[0];
            UnloadDirectoryFiles(fontDirectoryFiles);
        }else{
            UnloadDirectoryFiles(fontDirectoryFiles);
            return program_states::System::InitializationError::SoundFontMissing;
        }
    }
#else
    // std::string soundFontPath{GetApplicationDirectory()};
    // soundFontPath += constants::midi::SoundFontPath;
    const std::string rootDirectory{GetApplicationDirectory()};
    const std::string fontDirectory{rootDirectory + constants::midi::SoundFontDirectory};
    const std::string targetFontFile{constants::midi::SoundFontFile};

    std::string soundFontPath{fontDirectory + targetFontFile};

    if(!FileExists(soundFontPath.c_str())){
        FilePathList fontDirectoryFiles{LoadDirectoryFilesEx(fontDirectory.c_str(), constants::midi::SoundFontExtension, false)};
        if(fontDirectoryFiles.count > 0){
            soundFontPath = fontDirectoryFiles.paths[0];
            UnloadDirectoryFiles(fontDirectoryFiles);
        }else{
            UnloadDirectoryFiles(fontDirectoryFiles);
            FilePathList rootDirectoryFiles{LoadDirectoryFilesEx(rootDirectory.c_str(), constants::midi::SoundFontExtension, false)};
            if(rootDirectoryFiles.count > 0){
                soundFontPath = rootDirectoryFiles.paths[0];
                UnloadDirectoryFiles(rootDirectoryFiles);
            }else{
                UnloadDirectoryFiles(rootDirectoryFiles);
                return program_states::System::InitializationError::SoundFontMissing;
            }
        }
    }
#endif

    DEBUG_PRINT("soundFontPath: {}", soundFontPath);

    soundFont = tsf_load_filename(soundFontPath.c_str());
    
    if(!soundFont){
        return program_states::System::InitializationError::SoundFontLoadFailed;
    }

    tsf_set_max_voices(soundFont, constants::midi::MaximumVoiceCount);
    tsf_set_output(soundFont, TSF_STEREO_INTERLEAVED, outputSampleRate_, constants::midi::GlobalGainInDecibel);

    for(size_t channel{0}; channel < constants::project_data::NumberOfInstrumentChannels; channel++){
        applyInstrument(channel, context_.machine.instruments[channel]);
        applyVolume(channel);
        applyArticulation(channel);
    }
    setTempo(command::Tempo{context_.machine.tempo});

    outputAudioStream_ = LoadAudioStream(
        outputSampleRate_,
        constants::midi::OutputSampleSizeInBits,
        constants::midi::OutputChannelCount
    );

    SetAudioStreamCallback(outputAudioStream_, [](void *bufferData, unsigned int frames){
        std::lock_guard<std::mutex> lock(MidiManager::audioMutex);
        tsf_render_short(MidiManager::soundFont, static_cast<int16_t*>(bufferData), frames, 0);
    });

    // interleavedSampleBuffer_.resize(streamChunkFrameCount_ * 2);

    PlayAudioStream(outputAudioStream_);

    return program_states::System::InitializationError::None;
}

MidiManager::~MidiManager(){
    silence(command::Target::All_Channels);

    if(outputAudioStream_.buffer){
        StopAudioStream(outputAudioStream_);
        UnloadAudioStream(outputAudioStream_);
        outputAudioStream_ = AudioStream{};
    }

    if(soundFont){
        tsf_close(soundFont);
        soundFont = nullptr;
    }

    CloseAudioDevice();
}

void MidiManager::update(){

}

void MidiManager::noteOn(const command::Target channel, const music_data::Note note){
    if(channel == command::Target::All_Channels) return;
    activateNote(targetChannelToVectorIndex(channel), note);
    DEBUG_PRINT("NoteOn[{}] {}", magic_enum::enum_name(channel), magic_enum::enum_name(note));

}

void MidiManager::noteOff(const command::Target channel, const music_data::Note note){
    if(channel == command::Target::All_Channels) return;
    deactivateNote(targetChannelToVectorIndex(channel), note);
    DEBUG_PRINT("NoteOff[{}] {}", magic_enum::enum_name(channel), magic_enum::enum_name(note));
}

void MidiManager::silence(const command::Target channel){
    if(channel == command::Target::All_Channels) return;
    silenceChannel(targetChannelToVectorIndex(channel));
    DEBUG_PRINT("Silence[{}]", magic_enum::enum_name(channel));
}

void MidiManager::setInstrument(const command::Target channel, const music_data::Instrument instrument){
    if(channel == command::Target::All_Channels) return;
    applyInstrument(targetChannelToVectorIndex(channel), instrument);
    DEBUG_PRINT("Instrument[{}] set to {}", magic_enum::enum_name(channel), magic_enum::enum_name(instrument));
}

void MidiManager::setTempo(const command::Tempo tempo){
    context_.machine.tempo = tempo.tempo;
    DEBUG_PRINT("Tempo set to {}", tempo.tempo);
}

void MidiManager::setVolume(const command::Target channel, const units::machine::Volume volume){
    if(channel == command::Target::All_Channels){
        for(int instrumentChannelIndex{0}; instrumentChannelIndex < constants::project_data::NumberOfInstrumentChannels; instrumentChannelIndex++){
            context_.machine.volumes[instrumentChannelIndex] = volume;
            applyVolume(instrumentChannelIndex);
        }
    }else{
        const int instrumentChannelIndex{targetChannelToVectorIndex(channel)};
        context_.machine.volumes[instrumentChannelIndex] = volume;
        applyVolume(instrumentChannelIndex);
    }

    DEBUG_PRINT("Volume[{}] set to {}", magic_enum::enum_name(channel), volume);
}

void MidiManager::setArticulation(const command::Target channel, const units::machine::Articulation articulation){
    if(channel == command::Target::All_Channels){
        for(int instrumentChannelIndex{0}; instrumentChannelIndex < constants::project_data::NumberOfInstrumentChannels; instrumentChannelIndex++){
            context_.machine.articulations[instrumentChannelIndex] = articulation;
            applyArticulation(instrumentChannelIndex);
        }
    }else{
        const int instrumentChannelIndex{targetChannelToVectorIndex(channel)};
        context_.machine.articulations[instrumentChannelIndex] = articulation;
        applyArticulation(instrumentChannelIndex);
    }

    DEBUG_PRINT("Volume[{}] set to {}", magic_enum::enum_name(channel), magic_enum::enum_name(articulation));
}

