#include "prompts.hpp"

#include <algorithm>
#include <cmath>

#include "constants.hpp"

#include "labels.hpp"
#include "interface/utilities.hpp"

using namespace interface;
using namespace constants::labels::prompts;

const Vector2 &Prompts::windowAnchor(const program_states::Layout &layoutState, const program_states::Interface::Prompts &promptState){
    
    if(promptState.activeCommandPrompt == constants::prompts::CommandPrompt::Tempo) return layoutState.anchor.prompts.tempoWindow;
    if(promptState.activeCommandPrompt == constants::prompts::CommandPrompt::Volume) return layoutState.anchor.prompts.volumeWindow;

    return layoutState.anchor.prompts.articulationWindow;
}

Rectangle Prompts::calculateBoundsAtPromptAnchor(
    const program_states::Layout    &layoutState,
    const program_states::Interface &interfaceState,
    const Rectangle                 &relativeBounds
){

    return calculateBoundsAtAnchor(windowAnchor(layoutState, interfaceState.prompts), relativeBounds);
}

