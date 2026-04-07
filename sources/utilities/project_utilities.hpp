#pragma once

#include <algorithm>
#include <memory>
#include <optional>

#include "constants/project_data.hpp"

#include "program_states/project_data.hpp"
#include "program_states/system.hpp"

namespace utilities{

    inline std::shared_ptr<program_states::ProjectData> projectDataFrom(const program_states::System &systemState){
        const auto stagedSlot{systemState.project.data.lock()};
        if(!stagedSlot || !stagedSlot->data) return {};

        return stagedSlot->data;
    }

    inline std::shared_ptr<program_states::ProjectData> projectDataWithPagesFrom(const program_states::System &systemState){
        const auto projectData{projectDataFrom(systemState)};
        if(!projectData || projectData->pages.empty()) return {};

        return projectData;
    }

    inline int currentPageIndexFrom(const program_states::ProjectData &projectData, int currentPageNumber){
        if(projectData.pages.empty()) return 0;

        return std::clamp(
            currentPageNumber - 1,
            0, static_cast<int>(projectData.pages.size()) - 1
        );
    }

    inline int currentPageNoteCountFrom(const program_states::ProjectData &projectData, int currentPageNumber){
        if(projectData.pages.empty()) return constants::project_data::MinimumNotePerPage;

        const int currentPageIndex{currentPageIndexFrom(projectData, currentPageNumber)};
        return std::clamp(
            projectData.pages[currentPageIndex].noteInThisPage.value_or(projectData.metadata.notePerPage),
            constants::project_data::MinimumNotePerPage, constants::project_data::MaximumNotePerPage
        );
    }

    inline bool hasSpecificInstrumentChannelSelection(int selectedChannelListViewIndex){
        return selectedChannelListViewIndex >= 1
            && selectedChannelListViewIndex <= constants::project_data::NumberOfInstrumentChannels;
    }

    inline std::optional<int> selectedInstrumentChannelIndex(int selectedChannelListViewIndex){
        if(!hasSpecificInstrumentChannelSelection(selectedChannelListViewIndex)) return std::nullopt;

        return selectedChannelListViewIndex - 1;
    }

    inline int selectedChannelListIndexFromInstrumentChannelIndex(int instrumentChannelIndex){
        return instrumentChannelIndex + 1;
    }

    inline program_states::ProjectData::Page &pageByNumber(program_states::ProjectData &projectData, int pageNumber){
        return projectData.pages[pageNumber - 1];
    }
    inline const program_states::ProjectData::Page &pageByNumber(const program_states::ProjectData &projectData, int pageNumber){
        return projectData.pages[pageNumber - 1];
    }

    inline void applyTransientSelection(
        program_states::ProjectData::Transient &transient,
        int pageNumber,
        int instrumentChannelIndex
    ){
        transient.currentPageNumber = pageNumber;
        transient.selectedChannelListViewIndex = selectedChannelListIndexFromInstrumentChannelIndex(instrumentChannelIndex);
    }

} // namespace utilities
