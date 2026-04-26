#include "playback_manager.hpp"

command::Target PlaybackManager::channelIndexToChannelTarget(units::midi::SoundFontChannel channel){
    return static_cast<command::Target>(channel + 1);
}