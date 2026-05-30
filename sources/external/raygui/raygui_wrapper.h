#ifndef RAYGUI_WRAPPER_H
#define RAYGUI_WRAPPER_H

#include <raylib.h>

#include <stdbool.h>

extern float GuiTextBoxCursorCooldown;
extern float GuiTextBoxCursorSpeed;

int GuiTextBoxFPSIndependent(Rectangle bounds, char *text, int textSize, bool editMode);
int GuiScrollPanelOffset(Rectangle bounds, const char *text, Rectangle content, Vector2 *scroll, Rectangle *view);

#endif // RAYGUI_WRAPPER_H