#include "raygui_wrapper.h"

#include <raylib.h>
#include <raygui.h>

#include <string.h>
#include <ctype.h>

float GuiTextBoxCursorCooldown = 0.5f;
float GuiTextBoxCursorSpeed = 0.035f;

static int localTextBoxCursorIndex = 0; // replace raygui internal textBoxCursorIndex

// replace raygui internal GuiDrawRectangle()
static void WrapperDrawRectangle(Rectangle rec, int borderWidth, Color borderColor, Color color) {
    if (color.a > 0) DrawRectangleRec(rec, color);
    if (borderWidth > 0) DrawRectangleLinesEx(rec, (float)borderWidth, borderColor);
}

// replace raygui internal GuiDrawText()
static void WrapperDrawText(const char *text, Rectangle bounds, int alignment, Color tint) {
    Font font = GuiGetFont();
    float size = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
    float spacing = (float)GuiGetStyle(DEFAULT, TEXT_SPACING);
    Vector2 textSize = MeasureTextEx(font, text, size, spacing);
    
    float pad = (float)GuiGetStyle(TEXTBOX, TEXT_PADDING);
    
    Vector2 pos = { 
        (float)(int)(bounds.x + pad), 
        (float)(int)(bounds.y + bounds.height/2.0f - size/2.0f) 
    };
    
    if (alignment == TEXT_ALIGN_CENTER) pos.x = (float)(int)(bounds.x + bounds.width/2.0f - textSize.x/2.0f);
    else if (alignment == TEXT_ALIGN_RIGHT) pos.x = (float)(int)(bounds.x + bounds.width - textSize.x - pad);
    
    BeginScissorMode((int)bounds.x, (int)bounds.y, (int)bounds.width, (int)bounds.height);
    DrawTextEx(font, text, pos, size, spacing, tint);
    EndScissorMode();
}

int GuiTextBoxFPSIndependent(Rectangle bounds, char *text, int textSize, bool editMode)
{
    static float autoCursorTimer = 0.0f;
    static float autoCursorActionTimer = 0.0f;

    int result = 0;
    int state = GuiGetState(); 

    bool multiline = false; 
    int wrapMode = GuiGetStyle(DEFAULT, TEXT_WRAP_MODE);

    Rectangle textBounds = bounds;
    textBounds.x += GuiGetStyle(TEXTBOX, BORDER_WIDTH);
    textBounds.y += GuiGetStyle(TEXTBOX, BORDER_WIDTH);
    textBounds.width -= 2 * GuiGetStyle(TEXTBOX, BORDER_WIDTH);
    textBounds.height -= 2 * GuiGetStyle(TEXTBOX, BORDER_WIDTH);

    int textLength = (text != NULL) ? (int)strlen(text) : 0; 
    
    int thisCursorIndex = localTextBoxCursorIndex;
    if (thisCursorIndex > textLength) thisCursorIndex = textLength;
    
    Font font = GuiGetFont();
    float scaleFactor = (float)GuiGetStyle(DEFAULT, TEXT_SIZE) / (float)font.baseSize;
    
    float textWidth = MeasureTextEx(font, text, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING)).x;
    float cursorOffsetWidth = MeasureTextEx(font, text + thisCursorIndex, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING)).x;
    float drawnTextWidth = textWidth - cursorOffsetWidth;

    int textIndexOffset = 0; 

    Rectangle cursor = {
        textBounds.x + drawnTextWidth + GuiGetStyle(DEFAULT, TEXT_SPACING),
        textBounds.y + textBounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE),
        2,
        (float)GuiGetStyle(DEFAULT, TEXT_SIZE) * 2
    };

    if (cursor.height >= bounds.height) cursor.height = bounds.height - GuiGetStyle(TEXTBOX, BORDER_WIDTH)*2;
    if (cursor.y < (bounds.y + GuiGetStyle(TEXTBOX, BORDER_WIDTH))) cursor.y = bounds.y + GuiGetStyle(TEXTBOX, BORDER_WIDTH);

    Rectangle mouseCursor = cursor;
    mouseCursor.x = -1;
    mouseCursor.width = 1;

    if ((state != STATE_DISABLED) &&
        !GuiGetStyle(TEXTBOX, TEXT_READONLY) &&
        !GuiIsLocked() && 
        (wrapMode == TEXT_WRAP_NONE))
    {
        Vector2 mousePosition = GetMousePosition();

        if (editMode)
        {
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_BACKSPACE) || IsKeyDown(KEY_DELETE)) 
            {
                autoCursorTimer += GetFrameTime();
            }
            else 
            {
                autoCursorTimer = 0.0f;
                autoCursorActionTimer = 0.0f;
            }

            bool autoCursorShouldTrigger = false;
            if (autoCursorTimer >= GuiTextBoxCursorCooldown)
            {
                autoCursorActionTimer += GetFrameTime();
                if (autoCursorActionTimer >= GuiTextBoxCursorSpeed)
                {
                    autoCursorShouldTrigger = true;
                    autoCursorActionTimer -= GuiTextBoxCursorSpeed; 
                }
            }

            state = STATE_PRESSED;

            if (localTextBoxCursorIndex > textLength) localTextBoxCursorIndex = textLength;

            while (drawnTextWidth >= textBounds.width)
            {
                int nextCodepointSize = 0;
                GetCodepointNext(text + textIndexOffset, &nextCodepointSize);
                textIndexOffset += nextCodepointSize;
                
                float remainingWidth = MeasureTextEx(font, text + textIndexOffset, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING)).x;
                cursorOffsetWidth = MeasureTextEx(font, text + localTextBoxCursorIndex, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING)).x;
                drawnTextWidth = remainingWidth - cursorOffsetWidth;
            }

            int codepoint = GetCharPressed(); 
            if (multiline && IsKeyPressed(KEY_ENTER)) codepoint = (int)'\n';

            int codepointSize = 0;
            const char *charEncoded = CodepointToUTF8(codepoint, &codepointSize);

            if (IsKeyPressed(KEY_V) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)))
            {
                const char *pasteText = GetClipboardText();
                if (pasteText != NULL)
                {
                    int pasteLength = 0;
                    int pasteCodepoint;
                    int pasteCodepointSize;

                    while (true)
                    {
                        pasteCodepoint = GetCodepointNext(pasteText + pasteLength, &pasteCodepointSize);
                        if (textLength + pasteLength + pasteCodepointSize >= textSize) break;
                        if (!(multiline && (pasteCodepoint == (int)'\n')) && !(pasteCodepoint >= 32)) break;
                        pasteLength += pasteCodepointSize;
                    }

                    if (pasteLength > 0)
                    {
                        for (int i = textLength + pasteLength; i > localTextBoxCursorIndex; i--) text[i] = text[i - pasteLength];
                        for (int i = 0; i < pasteLength; i++) text[localTextBoxCursorIndex + i] = pasteText[i];

                        localTextBoxCursorIndex += pasteLength;
                        textLength += pasteLength;
                        text[textLength] = '\0';
                    }
                }
            }
            else if (((multiline && (codepoint == (int)'\n')) || (codepoint >= 32)) && ((textLength + codepointSize) < textSize))
            {
                for (int i = (textLength + codepointSize); i > localTextBoxCursorIndex; i--) text[i] = text[i - codepointSize];
                for (int i = 0; i < codepointSize; i++) text[localTextBoxCursorIndex + i] = charEncoded[i];

                localTextBoxCursorIndex += codepointSize;
                textLength += codepointSize;
                text[textLength] = '\0';
            }

            if ((textLength > 0) && IsKeyPressed(KEY_HOME)) localTextBoxCursorIndex = 0;
            if ((textLength > localTextBoxCursorIndex) && IsKeyPressed(KEY_END)) localTextBoxCursorIndex = textLength;

            if ((textLength > localTextBoxCursorIndex) && IsKeyPressed(KEY_DELETE) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)))
            {
                int offset = localTextBoxCursorIndex;
                int accCodepointSize = 0;
                int nextCodepointSize;
                int nextCodepoint;

                nextCodepoint = GetCodepointNext(text + offset, &nextCodepointSize);
                bool puctuation = ispunct(nextCodepoint & 0xff);
                while (offset < textLength)
                {
                    if ((puctuation && !ispunct(nextCodepoint & 0xff)) || (!puctuation && (isspace(nextCodepoint & 0xff) || ispunct(nextCodepoint & 0xff))))
                        break;
                    offset += nextCodepointSize;
                    accCodepointSize += nextCodepointSize;
                    nextCodepoint = GetCodepointNext(text + offset, &nextCodepointSize);
                }

                while (offset < textLength)
                {
                    if (!isspace(nextCodepoint & 0xff)) break;
                    offset += nextCodepointSize;
                    accCodepointSize += nextCodepointSize;
                    nextCodepoint = GetCodepointNext(text + offset, &nextCodepointSize);
                }

                for (int i = offset; i <= textLength; i++) text[i - accCodepointSize] = text[i];
                textLength -= accCodepointSize;
            }
            else if ((textLength > localTextBoxCursorIndex) && (IsKeyPressed(KEY_DELETE) || (IsKeyDown(KEY_DELETE) && autoCursorShouldTrigger)))
            {
                int nextCodepointSize = 0;
                GetCodepointNext(text + localTextBoxCursorIndex, &nextCodepointSize);
                for (int i = localTextBoxCursorIndex + nextCodepointSize; i <= textLength; i++) text[i - nextCodepointSize] = text[i];
                textLength -= nextCodepointSize;
            }

            if ((localTextBoxCursorIndex > 0) && IsKeyPressed(KEY_BACKSPACE) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)))
            {
                int offset = localTextBoxCursorIndex;
                int accCodepointSize = 0;
                int prevCodepointSize = 0;
                int prevCodepoint = 0;

                while (offset > 0)
                {
                    prevCodepoint = GetCodepointPrevious(text + offset, &prevCodepointSize);
                    if (!isspace(prevCodepoint & 0xff)) break;
                    offset -= prevCodepointSize;
                    accCodepointSize += prevCodepointSize;
                }

                bool puctuation = ispunct(prevCodepoint & 0xff);
                while (offset > 0)
                {
                    prevCodepoint = GetCodepointPrevious(text + offset, &prevCodepointSize);
                    if ((puctuation && !ispunct(prevCodepoint & 0xff)) || (!puctuation && (isspace(prevCodepoint & 0xff) || ispunct(prevCodepoint & 0xff)))) break;
                    offset -= prevCodepointSize;
                    accCodepointSize += prevCodepointSize;
                }

                for (int i = localTextBoxCursorIndex; i <= textLength; i++) text[i - accCodepointSize] = text[i];
                textLength -= accCodepointSize;
                localTextBoxCursorIndex -= accCodepointSize;
            }
            else if ((localTextBoxCursorIndex > 0) && (IsKeyPressed(KEY_BACKSPACE) || (IsKeyDown(KEY_BACKSPACE) && autoCursorShouldTrigger)))
            {
                int prevCodepointSize = 0;
                GetCodepointPrevious(text + localTextBoxCursorIndex, &prevCodepointSize);
                for (int i = localTextBoxCursorIndex; i <= textLength; i++) text[i - prevCodepointSize] = text[i];
                textLength -= prevCodepointSize;
                localTextBoxCursorIndex -= prevCodepointSize;
            }

            if ((localTextBoxCursorIndex > 0) && IsKeyPressed(KEY_LEFT) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)))
            {
                int offset = localTextBoxCursorIndex;
                int prevCodepointSize = 0;
                int prevCodepoint = 0;

                while (offset > 0)
                {
                    prevCodepoint = GetCodepointPrevious(text + offset, &prevCodepointSize);
                    if (!isspace(prevCodepoint & 0xff)) break;
                    offset -= prevCodepointSize;
                }

                bool puctuation = ispunct(prevCodepoint & 0xff);
                while (offset > 0)
                {
                    prevCodepoint = GetCodepointPrevious(text + offset, &prevCodepointSize);
                    if ((puctuation && !ispunct(prevCodepoint & 0xff)) || (!puctuation && (isspace(prevCodepoint & 0xff) || ispunct(prevCodepoint & 0xff)))) break;
                    offset -= prevCodepointSize;
                }

                localTextBoxCursorIndex = offset;
            }
            else if ((localTextBoxCursorIndex > 0) && (IsKeyPressed(KEY_LEFT) || (IsKeyDown(KEY_LEFT) && autoCursorShouldTrigger)))
            {
                int prevCodepointSize = 0;
                GetCodepointPrevious(text + localTextBoxCursorIndex, &prevCodepointSize);
                localTextBoxCursorIndex -= prevCodepointSize;
            }
            else if ((textLength > localTextBoxCursorIndex) && IsKeyPressed(KEY_RIGHT) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)))
            {
                int offset = localTextBoxCursorIndex;
                int nextCodepointSize;
                int nextCodepoint;

                nextCodepoint = GetCodepointNext(text + offset, &nextCodepointSize);
                bool puctuation = ispunct(nextCodepoint & 0xff);
                while (offset < textLength)
                {
                    if ((puctuation && !ispunct(nextCodepoint & 0xff)) || (!puctuation && (isspace(nextCodepoint & 0xff) || ispunct(nextCodepoint & 0xff)))) break;
                    offset += nextCodepointSize;
                    nextCodepoint = GetCodepointNext(text + offset, &nextCodepointSize);
                }

                while (offset < textLength)
                {
                    if (!isspace(nextCodepoint & 0xff)) break;
                    offset += nextCodepointSize;
                    nextCodepoint = GetCodepointNext(text + offset, &nextCodepointSize);
                }

                localTextBoxCursorIndex = offset;
            }
            else if ((textLength > localTextBoxCursorIndex) && (IsKeyPressed(KEY_RIGHT) || (IsKeyDown(KEY_RIGHT) && autoCursorShouldTrigger)))
            {
                int nextCodepointSize = 0;
                GetCodepointNext(text + localTextBoxCursorIndex, &nextCodepointSize);
                localTextBoxCursorIndex += nextCodepointSize;
            }

            if (CheckCollisionPointRec(mousePosition, textBounds))
            {
                int codepointIndex = 0;
                float glyphWidth = 0.0f;
                float widthToMouseX = 0;
                int mouseCursorIndex = 0;
                
                float textPadding = (float)GuiGetStyle(TEXTBOX, TEXT_PADDING);
                float textStartX = bounds.x + textPadding;

                for (int i = textIndexOffset; i < textLength; )
                {
                    int cpSize = 0;
                    int cp = GetCodepointNext(&text[i], &cpSize);
                    codepointIndex = GetGlyphIndex(font, cp);

                    if (font.glyphs[codepointIndex].advanceX == 0) glyphWidth = ((float)font.recs[codepointIndex].width*scaleFactor);
                    else glyphWidth = ((float)font.glyphs[codepointIndex].advanceX*scaleFactor);

                    if (mousePosition.x <= (textStartX + widthToMouseX + glyphWidth/2))
                    {
                        mouseCursor.x = (float)(int)(textStartX + widthToMouseX);
                        mouseCursorIndex = i;
                        break;
                    }

                    widthToMouseX += (glyphWidth + (float)GuiGetStyle(DEFAULT, TEXT_SPACING));
                    i += cpSize;
                }

                float textEndWidth = MeasureTextEx(font, text + textIndexOffset, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING)).x;
                
                if (mousePosition.x >= (textStartX + textEndWidth - glyphWidth/2))
                {
                    mouseCursor.x = (float)(int)(textStartX + textEndWidth);
                    mouseCursorIndex = textLength;
                }

                if ((mouseCursor.x >= 0) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    cursor.x = mouseCursor.x;
                    localTextBoxCursorIndex = mouseCursorIndex;
                }
            }
            else mouseCursor.x = -1;

            cursor.x = bounds.x + GuiGetStyle(TEXTBOX, TEXT_PADDING) + MeasureTextEx(font, text + textIndexOffset, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING)).x - MeasureTextEx(font, text + localTextBoxCursorIndex, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING)).x + GuiGetStyle(DEFAULT, TEXT_SPACING);

            if ((!multiline && IsKeyPressed(KEY_ENTER)) ||
                (!CheckCollisionPointRec(mousePosition, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
            {
                localTextBoxCursorIndex = 0;     
                autoCursorTimer = 0.0f;     
                autoCursorActionTimer = 0.0f;
                result = 1;
            }
        }
        else
        {
            if (CheckCollisionPointRec(mousePosition, bounds))
            {
                state = STATE_FOCUSED;

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    localTextBoxCursorIndex = textLength;   
                    autoCursorTimer = 0.0f;            
                    autoCursorActionTimer = 0.0f;
                    result = 1;
                }
            }
        }
    }

    if (state == STATE_PRESSED)
    {
        WrapperDrawRectangle(bounds, GuiGetStyle(TEXTBOX, BORDER_WIDTH), GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_NORMAL + (state*3))), GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_PRESSED)));
    }
    else if (state == STATE_DISABLED)
    {
        WrapperDrawRectangle(bounds, GuiGetStyle(TEXTBOX, BORDER_WIDTH), GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_NORMAL + (state*3))), GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_DISABLED)));
    }
    else WrapperDrawRectangle(bounds, GuiGetStyle(TEXTBOX, BORDER_WIDTH), GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_NORMAL + (state*3))), BLANK);

    WrapperDrawText(text + textIndexOffset, textBounds, GuiGetStyle(TEXTBOX, TEXT_ALIGNMENT), GetColor(GuiGetStyle(TEXTBOX, TEXT_COLOR_NORMAL + (state*3))));

    if (editMode && !GuiGetStyle(TEXTBOX, TEXT_READONLY))
    {
        WrapperDrawRectangle(cursor, 0, BLANK, GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_PRESSED)));
        if (mouseCursor.x >= 0) WrapperDrawRectangle(mouseCursor, 0, BLANK, GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_PRESSED)));
    }

    return result;
}