#pragma once

#include "program_states/context.hpp"

namespace interface{

	class Prompts{
	public:
		static void draw(program_states::InterfaceContext &context);

    private:
		static void drawTempoPrompt(program_states::InterfaceContext &context);
		static void drawVolumePrompt(program_states::InterfaceContext &context);
		static void drawArticulationPrompt(program_states::InterfaceContext &context);
		static void drawInstrumentPrompt(program_states::InterfaceContext &context);

	private:
		static void drawConstantsManagerPrompt(program_states::InterfaceContext &context);
		
		static void drawConstantsManagerCommitPrompt(program_states::InterfaceContext &context);
		static void drawConstantsManagerNotDefinedPrompt(program_states::InterfaceContext &context);

	private:
		static const Vector2 &windowAnchor(
            const program_states::Layout &layoutState, 
            const program_states::Interface::Prompts &promptState
        );
		static Rectangle calculateBoundsAtPromptAnchor(
			const program_states::Layout &layoutState,
			const program_states::Interface &interfaceState,
			const Rectangle &relativeBounds
		);

	private:
		static std::string instrumentListViewText(const std::vector<music_data::Instrument> &instruments);

	private:
		static void cleanRightPanel(program_states::InterfaceContext &context);

	private:
		static void commit(program_states::InterfaceContext &context);
		static void load(program_states::InterfaceContext &context);
	};

} // namespace interface

