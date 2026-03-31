#pragma once

#include <raylib.h>

namespace program_states{

    struct Layout{

        struct Anchor{

            struct Toolbar{
                Vector2 systemAction{  0, 0};
                Vector2 stateControl{248, 0};
                Vector2 toolPalette {448, 0};
                Vector2 toolOption  {784, 0};
            } toolbar{};

            struct Sidebar{
                Vector2 inspect     {0, 40};
            } sidebar{};

            struct NavigationBar{
                Vector2 transportControls   {416, 408};
                Vector2 perPageSettings     {648, 408};
                Vector2 timeline            {  0, 448};
                Vector2 status              {112, 408};
            } navigationBar{};

            struct NoteCanvas{
                Vector2 noteCanvas  {112, 40};
            } noteCanvas{};

        } anchor{};

        struct Bounds{

            struct Toolbar{

                struct SystemAction{
                    Rectangle groupBox          {0, 0, 248, 40};

                    Rectangle menuButton        { 16, 8, 56, 24};
                    Rectangle newFileButton     { 86, 8, 24, 24};
                    Rectangle loadFileButton    {114, 8, 24, 24};
                    Rectangle saveFileButton    {142, 8, 24, 24};
                    Rectangle exportFileButton  {170, 8, 24, 24};
                    Rectangle musicSettingButton{208, 8, 24, 24};
                } systemAction{};

                struct StateControl{
                    Rectangle groupBox      {0, 0, 200, 40};

                    Rectangle undoButton    {16, 8, 24, 24};
                    Rectangle redoButton    {44, 8, 24, 24};
                    Rectangle actionHistoryDropdownBox{80, 8, 104, 24};
                } stateControl{};

                struct ToolPalette{
                    Rectangle groupBox          {  0, 0, 336, 40};

                    Rectangle toolsToggleGroup  { 56, 8,  40, 24};
                    Rectangle toolsLabel        { 16, 8,  40, 24};
                    Rectangle cutNoteButton     {240, 8,  24, 24};
                    Rectangle copyNoteButton    {268, 8,  24, 24};
                    Rectangle pasteNoteButton   {296, 8,  24, 24};
                } toolPalette{};

                struct ToolOption{
                    Rectangle groupBox      {0, 0, 176, 40};
                } toolOption{};

            } toolbar{};

            struct Sidebar{

                struct Inspector{
                    Rectangle groupBox          { 0,  0, 112, 408};

                    Rectangle channelListView   { 8,  32, 96, 224};
                    Rectangle channelLabel      {32,   8, 48,  24};
                    Rectangle structureLine     { 8, 264, 96,  24};

                    Rectangle tempoValueBox     {48, 288, 56, 24};
                    Rectangle notesValueBox     {48, 320, 56, 24};
                    Rectangle barsValueBox      {48, 352, 56, 24};
                } inspector{};

            } sidebar{};

            struct NavigationBar{

                struct TransportControls{
                    Rectangle groupBox          {  0, 0, 232, 40};

                    Rectangle previousPageButton{ 32, 8,  24, 24};
                    Rectangle nextPageButton    { 60, 8,  24, 24};
                    Rectangle stopButton        {130, 8,  24, 24};
                    Rectangle playButton        {102, 8,  24, 24};
                    Rectangle pageRepeatToggle  {176, 8,  24, 24};
                } transportControls{};

                struct Timeline{
                    Rectangle groupBox          {  0,  0, 960, 96};

                    Rectangle scrollPanel       {112, 24, 776, 72};
                    Rectangle timelineLabel     {112,  0, 120, 24};

                    Rectangle pageNumberLabel   {816,  0,  72, 24};
                    Rectangle pageCopyButton    {896, 24,  24, 24};
                    Rectangle pagePasteButton   {928, 24,  24, 24};
                    Rectangle pageCutButton     {928, 56,  24, 24};
                    Rectangle pageSelectToggle  {896, 56,  24, 24};
                } timeline{};

                struct PerPageSettings{
                    Rectangle groupBox          {0, 0, 312, 40};

                    Rectangle notePerPageSpinner{208, 8, 88, 24};
                } perPageSettings{};

                struct Status{
                    Rectangle groupBox{0, 0, 304, 40};
                } status{};

            } navigationBar{};

            struct NoteCanvas{
                Rectangle groupBox{0, 0, 848, 368};
            } noteCanvas{};

        } bounds{};

    };


} // namespace program_states