#pragma once

#include <raylib.h>

namespace interface{

    inline Rectangle calculateBoundsAtAnchor(const Vector2 &anchor, const Rectangle &rectangle){
        return {anchor.x + rectangle.x, anchor.y + rectangle.y, rectangle.width, rectangle.height};
    }

} // namespace interface
