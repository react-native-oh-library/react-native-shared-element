#pragma once

#include "RNOH/Package.h"
#include "helper/Rect.h"

namespace rnoh {
class RNSharedElementContent {
public:
    std::shared_ptr<ComponentInstance> view;
    std::shared_ptr<drawing::Rect> size;

    static std::shared_ptr<drawing::Rect> getSize(std::shared_ptr<ComponentInstance> view) {
        // todo 待实现
        /*if (view instanceof GenericDraweeView) {
            GenericDraweeView imageView = (GenericDraweeView)view;
            DraweeController controller = imageView.getController();
            GenericDraweeHierarchy hierarchy = imageView.getHierarchy();
            if (controller == null || controller.toString().contains("fetchedImage=0,")) {
                return null;
            }
            RectF imageBounds = new RectF();
            hierarchy.getActualImageBounds(imageBounds);
            return imageBounds;
        } else if (view instanceof ImageView) {
            ImageView imageView = (ImageView)view;
            Drawable drawable = imageView.getDrawable();
            if (drawable == null)
                return null;
            int width = drawable.getIntrinsicWidth();
            int height = drawable.getIntrinsicHeight();
            if ((width <= 0) || (height <= 0)) {
                return null;
            }
            return new RectF(0, 0, width, height);
        }
        return new RectF(0, 0, view.getWidth(), view.getHeight());*/
        std::shared_ptr<drawing::Rect> rect = std::make_shared<drawing::Rect>(
            0, 0, view->getLocalRootArkUINode().getSavedWidth(), view->getLocalRootArkUINode().getSavedHeight());
        return rect;
    }
};
} // namespace rnoh
