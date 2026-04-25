#include "playback_manager.hpp"

void PlaybackManager::update(){

    if(context_.machine.isPlaying && context_.machine.isPlaying != wasPlaying_){
        setupPlayback();

    }

    wasPlaying_ = context_.machine.isPlaying;

}