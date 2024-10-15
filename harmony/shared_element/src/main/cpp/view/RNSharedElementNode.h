//
// Created on 2024/9/26.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".
#pragma once

#include "RNOH/CppComponentInstance.h"
#include "RNSharedElementContent.h"
#include "RNSharedElementNode.h"
#include "RNSharedElementStyle.h"
#include "SharedElementTransitionComponentDescriptor.h"

#include <memory>

namespace rnoh {

// todo
class Callback {};

class RNSharedElementNode {
public:
    RNSharedElementNode(int reactTag, std::shared_ptr<ComponentInstance> view, bool isParent,
                        std::shared_ptr<ComponentInstance> ancestor,
                        facebook::react::SharedElementNodeStyleCls styleConfig)
        : mReactTag(reactTag), mView(view), mIsParent(isParent), mAncestorView(ancestor), mStyleConfig(styleConfig) {
        RNSharedElementStyle style(styleConfig);
        mResolveStyle = style;
        DLOG(INFO) << "RNSharedElementNode init mReactTag: " << reactTag << " mView: " << mView
                   << " mIsParent: " << mIsParent << " mAncestorView: " << mAncestorView;
    }
    
    ~RNSharedElementNode(){
        DLOG(INFO) << "~RNSharedElementNode 析构";
        stopRetryLoop();
    }

    void addHideRef() {
        mHideRefCount++;
        if (mHideRefCount == 1) {
            mHideAlpha = 0; // mView.getAlpha();
            if (mView != nullptr) {
                mView->getLocalRootArkUINode().setOpacity(0.0);    
            }
        }
    }

    void releaseHideRef() {
        mHideRefCount--;
        if (mHideRefCount == 0) {
            if (mView != nullptr) {
                mView->getLocalRootArkUINode().setOpacity(mHideAlpha);    
            }
        }
    }

    int releaseRef() {
        if (--mRefCount == 0) {
            removeDraweeControllerListener(mResolvedView);
            stopRetryLoop();
            mView = nullptr;
            mAncestorView = nullptr;
            mResolvedView = nullptr;
            mContentCallbacks.clear();
            mStyleCallbacks.clear();
        }
        return mRefCount;
    }

    int getReactTag() { return mReactTag; }

    int addRef() { return ++mRefCount; }

    std::shared_ptr<ComponentInstance> getAncestorView() { return mAncestorView; }

    std::shared_ptr<ComponentInstance> getResolvedView() {
        if (mResolvedView != nullptr)
            return mResolvedView;
        std::shared_ptr<ComponentInstance> view = mView;
        if (mIsParent && mView != nullptr) {
            int childCount = mView->getChildren().size();
            if (childCount == 1) {
                view = mView->getChildren()[0];
            } else if (childCount <= 0) {
                DLOG(ERROR) << "Child for parent doesn't exist";
                return nullptr;
            }
        }
        mResolvedView = RNSharedElementNode::resolveView(view, mResolveStyle);
        return mResolvedView;
    }

    void requestStyle(std::function<void(std::shared_ptr<RNSharedElementStyle>)> callBack) {
        if (mStyleCache != nullptr) {
            callBack(mStyleCache);
            return;
        }
        mStyleCallbacks.push_back(callBack);
        if (!fetchInitialStyle()) {
            startRetryLoop();
        }
    }

    bool fetchInitialStyle() {
        std::shared_ptr<ComponentInstance> view = getResolvedView();
        if (view == nullptr)
            return false;
        if (mStyleCallbacks.size() == 0)
            return true;

        // Get relative size and position within parent
        int left = 0; // view.getLeft();
        int top = 0;  // view.getTop();
        int width = view->getLocalRootArkUINode().getSavedWidth();
        int height = view->getLocalRootArkUINode().getSavedHeight();
        if (width == 0 && height == 0)
            return false;
        std::shared_ptr<drawing::Matrix> transform = RNSharedElementStyle::getAbsoluteViewTransform(view);
        std::shared_ptr<drawing::Matrix> ancestorTransform = RNSharedElementStyle::getAbsoluteViewTransform(mAncestorView);
        if ((transform == nullptr) || (ancestorTransform == nullptr))
            return false;
        std::shared_ptr<drawing::Rect> frame = std::make_shared<drawing::Rect>(left, top, left + width, top + height);

        // Create style
        std::shared_ptr<RNSharedElementStyle> style = std::make_shared<RNSharedElementStyle>(mStyleConfig);
        RNSharedElementStyle::getLayoutOnScreen(view, style->layout);
        style->frame = frame;
        RNSharedElementStyle::getLayoutOnScreen(mAncestorView, style->ancestorLayout);
        style->ancestorTransform = ancestorTransform;

        // todo Get opacity
        style->opacity = 1; // view.getAlpha();

        // Update initial style cache
        mStyleCache = style;

        // Notify callbacks
        for (std::function<void(std::shared_ptr<RNSharedElementStyle>)> callback : mStyleCallbacks) {
            callback(style);
        }
        mStyleCallbacks.clear();
        return true;
    }

    void requestContent(std::function<void(std::shared_ptr<RNSharedElementContent>)> callBack) {
        if (mContentCache != nullptr) {
            callBack(mContentCache);
            return;
        }
        mContentCallbacks.push_back(callBack);
        if (!fetchInitialContent()) {
            startRetryLoop();
        }
    }


private:
    std::shared_ptr<ComponentInstance> mView;
    std::shared_ptr<ComponentInstance> mAncestorView;
    bool mIsParent;
    facebook::react::SharedElementNodeStyleCls mStyleConfig;
    RNSharedElementStyle mResolveStyle;
    std::shared_ptr<RNSharedElementStyle> mStyleCache;
    std::shared_ptr<ComponentInstance> mResolvedView;
    std::vector<std::function<void(std::shared_ptr<RNSharedElementStyle>)>> mStyleCallbacks;
    std::vector<std::function<void(std::shared_ptr<RNSharedElementContent>)>> mContentCallbacks;
    std::shared_ptr<RNSharedElementContent> mContentCache;
    int mReactTag;
    int mRefCount;
    int mHideRefCount;
    float mHideAlpha;

    std::function<void(RNSharedElementStyle)> CallBack;
    std::chrono::steady_clock::duration interval = std::chrono::milliseconds(static_cast<int>(std::round(8)));
    std::unique_ptr<std::thread> thread;
    std::atomic<bool> running = {false};

    bool fetchInitialContent() {
        std::shared_ptr<ComponentInstance> view = getResolvedView();
        //DLOG(INFO) << "getResolvedView view: " << view;
        if (view == nullptr)
            return false;
        if (mContentCallbacks.size() == 0)
            return true;

        // Verify view size
        int width = view->getLocalRootArkUINode().getSavedWidth();
        int height = view->getLocalRootArkUINode().getSavedHeight();
        if (width == 0 && height == 0)
            return false;

        // Get content size (e.g. the size of the underlying image of an image-view)
        std::shared_ptr<drawing::Rect> contentSize = RNSharedElementContent::getSize(view);
        if (contentSize == nullptr) {
            // Image has not yet been fetched, listen for it
            addDraweeControllerListener(view);
            return false;
        }

        // Create content
        std::shared_ptr<RNSharedElementContent> content = std::make_shared<RNSharedElementContent>();
        content->view = view;
        content->size = contentSize;

        // Update cache
        mContentCache = content;

        // Notify callbacks
        for (std::function<void(std::shared_ptr<RNSharedElementContent>)> callback : mContentCallbacks) {
            callback(content);
        }
        mContentCallbacks.clear();
        return true;
    }

    void addDraweeControllerListener(std::shared_ptr<ComponentInstance> view) {
        // todo
        /*if (mDraweeControllerListener != null)
            return;

        if (!(view instanceof GenericDraweeView))
            return;
        GenericDraweeView imageView = (GenericDraweeView)view;
        DraweeController controller = imageView.getController();
        if (!(controller instanceof PipelineDraweeController))
            return;
        PipelineDraweeController pipelineController = (PipelineDraweeController)controller;

        mDraweeControllerListener = new BaseControllerListener<ImageInfo>() {
            @Override public void onSubmit(String id, Object callerContext) {
                // Log.d(LOG_TAG, "mDraweeControllerListener.onSubmit: " + id + ", callerContext: " + callerContext);
            }

            @Override public void onFinalImageSet(String id, @Nullable final ImageInfo imageInfo,
                                                  @Nullable Animatable animatable) {
                // Log.d(LOG_TAG, "mDraweeControllerListener.onFinalImageSet: " + id + ", imageInfo: " + imageInfo);
                removeDraweeControllerListener(view);
                fetchInitialContent();
            }

            @Override public void onFailure(String id, Throwable throwable) {
                Log.d(LOG_TAG, "mDraweeControllerListener.onFailure: " + id + ", throwable: " + throwable);
            }
        };

        pipelineController.addControllerListener(mDraweeControllerListener);*/
    }

    void removeDraweeControllerListener(std::shared_ptr<ComponentInstance> view) {
        // todo
        /*if (mDraweeControllerListener == null)
            return;
        GenericDraweeView imageView = (GenericDraweeView)view;
        DraweeController controller = imageView.getController();
        if (!(controller instanceof PipelineDraweeController))
            return;
        PipelineDraweeController pipelineController = (PipelineDraweeController) controller;
        pipelineController.removeControllerListener(mDraweeControllerListener);
        mDraweeControllerListener = null;*/
    }

    static std::shared_ptr<ComponentInstance> resolveView(std::shared_ptr<ComponentInstance> view,
                                                          RNSharedElementStyle style) {
        if (view == nullptr)
            return nullptr;

        int viewWidth = view->getLocalRootArkUINode().getSavedWidth();
        int viewHeight = view->getLocalRootArkUINode().getSavedHeight();
        // If the view is a ViewGroup and it contains exactly one
        // imageview with the same size, then use that imageview
        int childCount = view->getChildren().size();
        if (childCount > 0) {
            if (childCount == 1) {
                std::shared_ptr<ComponentInstance> childView = view->getChildren()[0];
                if (childView->getComponentName() == "Image") {
                    int left = 0; // childView.getLeft();
                    int top = 0;  // childView.getTop();
                    int width = childView->getLocalRootArkUINode().getSavedWidth();
                    int height = childView->getLocalRootArkUINode().getSavedHeight();

                    int expectedLeft = lround(style.borderWidth);
                    int expectedTop = lround(style.borderWidth);
                    int expectedWidth = lround((float)viewWidth - (style.borderWidth * 2));
                    int expectedHeight = lround((float)viewHeight - (style.borderWidth * 2));
                    if (((left >= expectedLeft - 1) && (left <= expectedLeft + 1)) &&
                        ((top >= expectedTop - 1) && (top <= expectedTop + 1)) &&
                        ((width >= expectedWidth - 1) && (width <= expectedWidth + 1)) &&
                        ((height >= expectedHeight - 1) && (height <= expectedHeight + 1))) {
                        return childView;
                    }
                }
            }
        }
        return view;
    }

    void startRetryLoop() {
        if (!running) {
            running = true;
            thread = std::make_unique<std::thread>(&RNSharedElementNode::postFrameLoop, this);
        }
    }

    void stopRetryLoop() {
        if (running) {
            running = false;
            if (thread->joinable()) {
                thread->join();
            }
            thread.reset();
        }
    }

    void postFrameLoop() {
        auto lastTime = std::chrono::steady_clock::now();
        auto nextTime = lastTime + interval;
        while (running) {
            std::this_thread::sleep_until(nextTime);
            auto currentTime = std::chrono::steady_clock::now();
            lastTime = currentTime; // 更新 lastTime 为当前时间

            bool isContentFetched = fetchInitialContent();
            bool isStyleFetched = fetchInitialStyle();
            if (isContentFetched && isStyleFetched) {
                running = false;
            }
            // 重新计算下一次运行的时间点
            nextTime = lastTime + interval;
        }
    }
};
}; // namespace rnoh
