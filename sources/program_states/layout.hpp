#pragma once

#include <raylib.h>

#include "constants/prompts.hpp"

namespace program_states{

    struct Layout{

        struct Anchor{

            struct Toolbar{
                Vector2 systemAction{  0, 0};
                Vector2 stateControl{280, 0};
                Vector2 toolPalette {464, 0};
                Vector2 toolOption  {800, 0};
            } toolbar{};

            struct Sidebar{
                Vector2 inspect     {0, 40};
            } sidebar{};

            struct NavigationBar{
                Vector2 transportControls   {416, 408};
                Vector2 perPageSettings     {648, 408};
                Vector2 status              {112, 408};
                
                Vector2 timeline            {  0, 448};
            } navigationBar{};

            struct NoteCanvas{
                Vector2 noteCanvas  {112, 40};
            } noteCanvas{};

            struct Prompts{
                Vector2 tempoWindow         {0, 0};
                Vector2 volumeWindow        {0, 0};
                Vector2 articulationWindow  {0, 0};
                Vector2 instrumentWindow    {0, 0};

                Vector2 constantsManagerWindow          {0, 0};
                Vector2 constantsManagerWarningWindow   {0, 0};
                Vector2 constantsManagerInfoWindow      {0, 0};

                Vector2 overwriteWarningWindow          {0, 0};
                Vector2 infoWindow                      {0, 0};
                Vector2 musicSettingWindow              {0, 0};
            } prompts{};

        } anchor{};

        struct Bounds{

            struct Toolbar{

                struct SystemAction{
                    Rectangle groupBox          {0, 0, 280, 40};

                    Rectangle menuButton        { 16, 8, 56, 24};
                    Rectangle newFileButton     { 86, 8, 24, 24};
                    Rectangle loadFileButton    {114, 8, 24, 24};
                    Rectangle saveFileButton    {142, 8, 24, 24};
                    Rectangle saveAsFileButton  {171, 8, 24, 24};
                    Rectangle exportFileButton  {200, 8, 24, 24};
                    Rectangle musicSettingButton{240, 8, 24, 24};
                } systemAction{};

                struct StateControl{
                    Rectangle groupBox      {0, 0, 184, 40};

                    Rectangle undoButton    {16, 8, 24, 24};
                    Rectangle redoButton    {44, 8, 24, 24};
                    Rectangle constantsButton{80,8, 88, 24};
                } stateControl{};

                struct ToolPalette{
                    Rectangle groupBox          {  0, 0, 336, 40};

                    Rectangle toolsToggleGroup  { 56, 8,  40, 24};
                    Rectangle openCommandWindowButton{184, 8, 20, 24};
                    Rectangle openInstrumentWindowButton{222, 8, 10, 24};
                    Rectangle toolsLabel        { 16, 8,  40, 24};
                    Rectangle cutNoteButton     {240, 8,  24, 24};
                    Rectangle copyNoteButton    {268, 8,  24, 24};
                    Rectangle pasteNoteButton   {296, 8,  24, 24};

                    Rectangle constantExitButton{ 16, 8, 210, 24};
                } toolPalette{};

                struct ToolOption{
                    Rectangle groupBox      {  0, 0, 160, 40};
                    Rectangle infoButton    {120, 8,  24, 24};
                    Rectangle readOnlyIcon  { 96, 8,  24, 24};
                } toolOption{};

            } toolbar{};

            struct Sidebar{

                struct Inspector{
                    Rectangle groupBox          { 0,  0, 112, 408};

                    Rectangle channelListView   { 8,  32, 96, 224};
                    Rectangle channelLabel      {32,   8, 48,  24};
                    // Rectangle structureLine     { 8, 264, 96,  24}; // "Global" label
                    // Rectangle tempoValueBox     {48, 288, 56, 24};
                    // Rectangle notesValueBox     {48, 320, 56, 24};
                    Rectangle sideSettingLine   { 8, 264, 96, 24};
                    Rectangle showCommandsCheckBox{8,288, 24, 24};
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

                    Rectangle scrollPanel       {120, 24, 768, 72};
                    Rectangle timelineLabel     {120,  0, 120, 24};

                    Rectangle addPageButton     {788,  4,  24, 16};
                    Rectangle pageNumberLabel   {816,  0,  32, 24};
                    Rectangle pageNumberValueLabel{848,0,  72, 24};
                    Rectangle pageCopyButton    {896, 24,  24, 24};
                    Rectangle pagePasteButton   {928, 24,  24, 24};
                    Rectangle pageCutButton     {928, 56,  24, 24};
                    Rectangle pageSelectToggle  {896, 56,  24, 24};
                    Rectangle statusBar         {  0,  0, 112, 96};
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

            struct Prompts{
                struct ConstantsManager{
                    Rectangle windowBox             {0,  0, 688, 288};

                    Rectangle editConstantGroupBox  {432, 32, 248, 40};
                    Rectangle textConstantLabel     {440, 40,  32, 24};
                    Rectangle editConstantTextBox   {472, 40, 192, 24};
                    Rectangle typeToggleGroup       {440, 80,  56, 24};

                    Rectangle constantsScrollPanel  {8, 55, 416, 225};
                    Rectangle constantsContentPanel {
                        8, 55, 402, 
                        constants::project_data::MaximumNumberOfConstants * 24
                    };

                    Rectangle idBar                 {  8, 32,  25, 24};
                    Rectangle nameBar               { 32, 32, 161, 24};
                    Rectangle typeBar               {192, 32,  89, 24};
                    Rectangle valueBar              {280, 32, 144, 24};

                    // Rectangle constantsPanel    {  8, 72, 408, 240};
                    Rectangle constantItemPanel {  9, 56, 402,  24};
                    Rectangle listIdLabel       { 16, 56,  16,  24};
                    Rectangle listTypeLabel     {200, 56,  80,  24};
                    Rectangle listValueLabel    {288, 56, 120,  24};
                    Rectangle listNameLabel     { 40, 56, 152,  24};

                    Rectangle tempoGroupBox     {432, 112, 248, 88};
                    Rectangle targetGroupBox    {432, 112, 248, 40};
                    Rectangle targetToggleGroup {496, 120,  32, 24};
                    Rectangle targetChannelLabel{440, 120,  56, 24};
                    Rectangle tempoSlider       {440, 144, 224, 16};
                    Rectangle tempoTextLabel    {440, 128, 224, 16};
                    Rectangle tempoPercentageLabel{496, 128, 224, 16};

                    Rectangle commitButton      {600, 256, 80, 24};
                    Rectangle revertButton      {504, 256, 80, 24};

                    Rectangle swapGroupBox      {504, 208, 176, 40};
                    Rectangle swapButton        {624, 216,  40, 24};
                    Rectangle swapLeftLabel     {512, 216,  24, 24};
                    Rectangle swapLeftTextBox   {536, 216,  24, 24};
                    Rectangle swapRightTextBox  {592, 216,  24, 24};
                    Rectangle swapRightLabel    {562, 216,  32, 24};

                    Rectangle articulationGroupBox     {432, 160, 248, 40};
                    Rectangle articulationToggleGroup  {448, 168,  40, 24};

                    Rectangle volumeGroupBox    {432, 160, 248, 40};
                    Rectangle volumeToggleGroup {488, 168,  16, 24};
                    Rectangle volumeLabel       {440, 168,  48, 24};

                    Rectangle loadButton        {432, 208,  64, 72};
                } constantsManager{};

                struct ConstantsManagerWarning{
                    Rectangle windowBox {  0,  0, 272, 80};
                    Rectangle label     {  8, 24, 256, 24};
                    Rectangle yesButton {160, 48,  64, 24};
                    Rectangle noButton  { 48, 48,  64, 24};
                } constantsManagerWarning{};

                struct ConstantsManagerInfo{
                    Rectangle windowBox { 0,  0, 160, 80};
                    Rectangle label     { 8, 24, 152, 24};
                    Rectangle okButton  {48, 48,  64, 24};
                } constantsManagerInfo{};

                struct OverwriteWarning{
                    Rectangle windowBox {  0,  0, 272, 80};
                    Rectangle label     {  8, 24, 256, 24};
                    Rectangle yesButton {160, 48,  64, 24};
                    Rectangle noButton  { 48, 48,  64, 24};
                } overwriteWarning{};

                struct InfoWindow{
                    Rectangle windowBox           {  0,   0, 368, 184};
                    Rectangle titleLabel          { 16,  24, 336,  24};
                    Rectangle infoLine            {  8,  40, 352,  12};
                    Rectangle repoLink1Button     { 16,  80, 392,  16};
                    Rectangle repoLink2Button     { 16,  96, 376,  16};
                    Rectangle descriptionLabel    { 16,  48, 360,  24};
                    Rectangle poweredByLabel      { 16, 120, 312,  24};
                    Rectangle librariesLabel      { 24, 136, 328,  16};
                    Rectangle timestampLabel      {152,  24, 200,  24};
                    Rectangle copyrightedBar      {  0, 160, 368,  24};
                } infoWindow{};

                struct MusicSetting{
                    Rectangle windowBox           {  0,   0, 248, 224};
                    
                    Rectangle titleLabel          {  8,  32, 120,  24};
                    Rectangle titleTextBox        { 64,  32, 176,  24};

                    Rectangle authorLabel         {  8,  56, 120,  24};
                    Rectangle authorTextBox       { 64,  56, 176,  24};

                    Rectangle createdLabel        {  8,  80, 120,  24};
                    Rectangle createdDateLabel    { 64,  80, 176,  24};

                    Rectangle modifiedLabel       {  8, 104, 120,  24};
                    Rectangle modifiedDateLabel   { 64, 104, 176,  24};

                    Rectangle versionLabel        {  8, 128, 120,  24};
                    Rectangle versionDateLabel    { 64, 128, 120,  24};

                    Rectangle readOnlyCheckBox    {152, 128,  24,  24};

                    Rectangle line                {  8, 152, 232,  16};

                    Rectangle tempoLabel          {  8, 168, 144,  24};
                    Rectangle tempoValueBox       {152, 168,  88,  24};

                    Rectangle notesLabel          {  8, 192, 144,  24};
                    Rectangle notesValueBox       {152, 192,  88,  24};
                } musicSetting{};

                struct Tempo{
                    Rectangle windowBox         {  0,   0, 320, 136};
                    Rectangle groupBox          { 16,  32, 288,  64};
                    Rectangle slider            {112,  40, 120,  16};
                    Rectangle percentageValueBox{240,  40,  32,  16};
                    Rectangle percentageLabel   {272,  40,  16,  16};
                    Rectangle descriptionLine   { 24,  56, 272,  16};
                    Rectangle descriptionLabel  { 24,  72, 272,  16};
                    Rectangle backButton        { 56, 104,  88,  24};
                    Rectangle confirmButton     {176, 104,  88,  24};
                } tempo{};

                struct Volume{
                    Rectangle windowBox         {  0,   0, 448, 136};
                    Rectangle groupBox          { 16,  32, 208,  40};
                    Rectangle levelToggleGroup  { 32,  40,  16,  24};
                    Rectangle targetGroupBox    {232,  32, 200,  40};
                    Rectangle targetLabel       {240,  40,  48,  24};
                    Rectangle targetToggleGroup {288,  40,  24,  24};
                    Rectangle descriptionLabel  { 16,  80, 416,  16};
                    Rectangle backButton        {120, 104,  88,  24};
                    Rectangle confirmButton     {240, 104,  88,  24};
                } volume{};

                struct Articulation{
                    Rectangle windowBox         {  0,   0, 432, 272};
                    Rectangle groupBox          { 16,  32, 400, 152};
                    Rectangle stateToggleGroup  { 32, 152,  72,  24};
                    Rectangle line              { 24, 136, 384,  16};
                    Rectangle stateLabel        { 40,  42,  64,  96};
                    Rectangle descriptionLabel  {104,  42, 296,  96};
                    Rectangle targetGroupBox    { 16, 192, 400,  40};
                    Rectangle targetToggleGroup { 32, 200,  72,  24};
                    Rectangle backButton        {112, 240,  88,  24};
                    Rectangle confirmButton     {232, 240,  88,  24};
                } articulation{};

                struct Instrument{
                    Rectangle windowBox     {  0,  0, 320, 240};
                    Rectangle familyListView{  8, 48,  88, 184};
                    Rectangle memberListView{104, 48, 208, 184};
                    Rectangle familyLabel   { 32, 24,  40,  24};
                    Rectangle memberLabel   {184, 24,  48,  24};
                } instrument{};
                
            } prompts{};

        } bounds{};

    };


} // namespace program_states