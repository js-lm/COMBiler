#pragma once

namespace constants::toolbar{

    enum class Tool{
        Cursor,
        Pen,
        Eraser,
        Change_Instrument
    };

    constexpr int ToolCount{4};

    constexpr int toIndex(Tool selectedTool){
        return static_cast<int>(selectedTool);
    }

    inline Tool toolFromIndex(int toolIndex){
        switch(toolIndex){
        case toIndex(Tool::Cursor): return Tool::Cursor;
        case toIndex(Tool::Pen): return Tool::Pen;
        case toIndex(Tool::Eraser): return Tool::Eraser;
        default: return Tool::Change_Instrument;
        }
    }

    constexpr float ReadOnlyBlinkDurationInSeconds{2.0f};
    constexpr int ReadOnlyBlinkFrequency{10};

} // namespace constants::toolbar
