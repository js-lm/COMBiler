#include "midi_manager.hpp"

#include <string>

#include "constants.hpp"

#include "debug_utilities.hpp"

void MidiManager::initialization(){

    DEBUG_PRINT("IsAudioDeviceReady(): {}", IsAudioDeviceReady());

    InitAudioDevice();

    std::string soundFontPath{GetApplicationDirectory()};
    soundFontPath += constants::midi::SoundFontPath;

    DEBUG_PRINT("soundFontPath: {}", soundFontPath);

    soundFont = tsf_load_filename(soundFontPath.c_str());

    tsf_set_max_voices(soundFont, constants::midi::MaximumVoiceCount);
    tsf_set_output(soundFont, TSF_STEREO_INTERLEAVED, outputSampleRate_, constants::midi::GlobalGainInDecibel);

    outputAudioStream_ = LoadAudioStream(
        outputSampleRate_,
        constants::midi::OutputSampleSizeInBits,
        constants::midi::OutputChannelCount
    );

    // TODO: race condition risk
    SetAudioStreamCallback(outputAudioStream_, [](void *bufferData, unsigned int frames){
        tsf_render_short(MidiManager::soundFont, static_cast<int16_t*>(bufferData), frames, 0);
    });

    // interleavedSampleBuffer_.resize(streamChunkFrameCount_ * 2);

    PlayAudioStream(outputAudioStream_);
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
}

void MidiManager::noteOff(const command::Target channel, const music_data::Note note){
    if(channel == command::Target::All_Channels) return;
    deactivateNote(targetChannelToVectorIndex(channel), note);
}

void MidiManager::silence(const command::Target channel){
    if(channel == command::Target::All_Channels) return;
    silenceChannel(targetChannelToVectorIndex(channel));
}

void MidiManager::setInstrument(const command::Target channel, const music_data::Instrument instrument){
    if(channel == command::Target::All_Channels) return;
    applyInstrument(targetChannelToVectorIndex(channel), instrument);
}

void MidiManager::setTempo(const command::Tempo tempo){
    context_.machine.tempo = tempo;
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
}

