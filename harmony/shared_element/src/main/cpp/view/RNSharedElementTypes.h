#pragma once
#include <__errc>
#include <stdexcept>

namespace rnoh {

enum class RNSharedElementAnimation { MOVE = 0, FADE = 1, FADE_IN = 2, FADE_OUT = 3 };

enum class RNSharedElementResize { AUTO = 0, STRETCH = 1, CLIP = 2, NONE = 3 };

enum class RNSharedElementAlign {
    AUTO = 0,
    LEFT_TOP = 1,
    LEFT_CENTER = 2,
    LEFT_BOTTOM = 3,
    RIGHT_TOP = 4,
    RIGHT_CENTER = 5,
    RIGHT_BOTTOM = 6,
    CENTER_TOP = 7,
    CENTER_CENTER = 8,
    CENTER_BOTTOM = 9
};

// copy from android.widget.ImageView.ScaleType
enum class ScaleType {
    MATRIX = 0,
    FIT_XY = 1,
    FIT_START = 2,
    FIT_CENTER = 3,
    FIT_END = 4,
    CENTER = 5,
    CENTER_CROP = 6,
    CENTER_INSIDE = 7
};

} // namespace rnoh