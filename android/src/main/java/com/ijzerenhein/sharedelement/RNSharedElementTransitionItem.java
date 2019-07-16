package com.ijzerenhein.sharedelement;

import android.view.View;
import android.view.ViewParent;
import android.graphics.Rect;

class RNSharedElementTransitionItem {
    private RNSharedElementNodeManager mNodeManager;
    private String mName;
    private boolean mIsAncestor;
    private RNSharedElementNode mNode;
    private boolean mHidden;
    private boolean mNeedsStyle;
    private RNSharedElementStyle mStyle;
    private boolean mNeedsContent;
    private RNSharedElementContent mContent;
    private Rect mClippedLayoutCache;

    RNSharedElementTransitionItem(RNSharedElementNodeManager nodeManager, String name, boolean isAncestor) {
        mNodeManager = nodeManager;
        mNode = null;
        mName = name;
        mIsAncestor = isAncestor;
        mHidden = false;
        mNeedsStyle = false;
        mStyle = null;
        mNeedsContent = false;
        mContent = null;
        mClippedLayoutCache = null;
    }

    String getName() {
        return mName;
    }

    boolean isAncestor() {
        return mIsAncestor;
    }

    void setHidden(boolean hidden) {
        if (mHidden == hidden) return;
        mHidden = hidden;
        if (hidden) {
            mNode.addHideRef();
        } else {
            mNode.releaseHideRef();
        }
    }

    boolean getHidden() {
        return mHidden;
    }

    RNSharedElementNode getNode() {
        return mNode;
    }

    void setNode(RNSharedElementNode node) {
        if (mNode == node) {
            if (node != null) mNodeManager.release(node);
            return;
        }
        if (mNode != null) {
            if (mHidden) mNode.releaseHideRef();
            mNodeManager.release(mNode);
        }
        mNode = node;
        mHidden = false;
        mNeedsStyle = node != null;
        mStyle = null;
        mNeedsContent = !mIsAncestor && (node != null);
        mContent = null;
    }

    boolean getNeedsStyle() {
        return mNeedsStyle;
    }

    void setNeedsStyle(boolean needsStyle) {
        mNeedsStyle = needsStyle;
    }

    void setStyle(RNSharedElementStyle style) {
        mStyle = style;
    }

    RNSharedElementStyle getStyle() {
        return mStyle;
    }

    boolean getNeedsContent() {
        return mNeedsContent;
    }

    void setNeedsContent(boolean needsContent) {
        mNeedsContent = needsContent;
    }

    void setContent(RNSharedElementContent content) {
        mContent = content;
    }

    RNSharedElementContent getContent() {
        return mContent;
    }

    View getView() {
        return (mNode != null) ? mNode.getResolvedView() : null;
    }

    Rect getClippedLayout(RNSharedElementTransitionItem ancestor) {
        if (mClippedLayoutCache != null) return mClippedLayoutCache;
        if ((mStyle == null) || (ancestor == null)) return null;
        View view = getView();
        View ancestorView = ancestor.getView();

        Rect clippedLayout = new Rect(mStyle.layout);
        
        /*Rect clippedLayout = new Rect(mStyle.layout);
        if (!view.getGlobalVisibleRect(clippedLayout)) {
            clippedLayout.right = clippedLayout.left;
            clippedLayout.bottom = clippedLayout.top; 
        }*/
        // Get visible area (some parts may be clipped in a scrollview or something)
        /*Rect clippedLayout = new Rect(mStyle.layout);
        ViewParent parentView = view.getParent();
        while (parentView != null) {
            Rect bounds = new Rect(view.getLeft(), view.getTop(), view.getRight(), view.getBottom());
            parentView.getChildVisibleRect(view, bounds, null);
            if (!clippedLayout.intersect(bounds)) {
                clippedLayout.right = clippedLayout.left;
                clippedLayout.bottom = clippedLayout.top; 
                break;
            }
            if (parentView == ancestorView) break;
            view = parentView;
            parentView = parentView.getParent();
        }*/

        mClippedLayoutCache = clippedLayout;
        return clippedLayout;
    }
}