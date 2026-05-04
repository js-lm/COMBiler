#include "enumerator.hpp"

Enumerator::PDF Enumerator::print(
    const program_states::ProjectData   &projectData, 
    const units::enumerator::Paper      paper,
    const units::Mm                     margin
){
    compiledData_ = compileProjectData(projectData);

    if(compiledData_.empty()) return {};






    return {};
}