#pragma once

#include <string>

namespace constants::serializer{

    constexpr const int Version{0};

    constexpr const char *SaveDialogTitle{"Save Your Project"};
    constexpr const char *LoadDialogTitle{"Select Your Project"};

    constexpr const char *SaveFileExtension{"cmb"};
    constexpr const char *FilterDescription{"COMBiler save files (*.cmb)"};

    constexpr const char *DefaultFilename{"COMBosition.cmb"};

    // web
    constexpr const char *IdbfsMountPoint{"/local"};
    constexpr const char *LastProjectFile{"/local/.last_project"};

} // namespace constants::serializer