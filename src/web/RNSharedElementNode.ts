import { RNSharedElementStyle } from "./RNSharedElementStyle";
import { RNSharedElementContent } from "./RNSharedElementContent";
import { Rect } from "./Rect";

export type RNSharedElementNodeStyleCallback = (
  value: RNSharedElementStyle
) => void;
export type RNSharedElementNodeContentCallback = (
  value: RNSharedElementContent
) => void;

export class RNSharedElementNode {
  public readonly domNode: HTMLElement;
  public readonly ancestorDomNode: HTMLElement;
  public readonly isParent: boolean;
  private hideRefCount: number = 0;
  private hideOpacity: number = 0;
  private refCount: number = 1;
  private styleCache: RNSharedElementStyle | null = null;
  private styleCallbacks: RNSharedElementNodeStyleCallback[] | null = null;
  private contentCache: RNSharedElementContent | null = null;
  private contentCallbacks: RNSharedElementNodeContentCallback[] | null = null;

  constructor(
    domNode: HTMLElement,
    isParent: boolean,
    ancestorDomNode: HTMLElement
  ) {
    this.domNode = domNode;
    this.isParent = isParent;
    this.ancestorDomNode = ancestorDomNode;
  }

  addRef() {
    return ++this.refCount;
  }

  releaseRef() {
    return --this.refCount;
  }

  addHideRef() {
    this.hideRefCount++;
    if (this.hideRefCount === 1) {
      const element: any = this.resolvedElement;
      this.hideOpacity = element.style.opacity;
      element.style.opacity = 0;
    }
  }

  releaseHideRef() {
    this.hideRefCount--;
    if (this.hideRefCount === 0) {
      const element: any = this.resolvedElement;
      element.style.opacity = this.hideOpacity;
    }
  }

  private static resolveElement(element: HTMLElement): HTMLElement {
    // TODO
    return element;
  }

  get resolvedElement(): HTMLElement | null {
    let element: HTMLElement = this.domNode;
    if (this.isParent) {
      const node: any = element;
      if (node.childNodes.length === 1) {
        return node.childNodes[0];
      } else if (node.childNodes.length <= 0) {
        console.log("Child for parent doesnt exist");
        return null;
      }
    }
    return RNSharedElementNode.resolveElement(element);
  }

  get resolvedAncestor(): HTMLElement | null {
    return this.ancestorDomNode;
  }

  requestStyle(): Promise<RNSharedElementStyle> {
    if (this.styleCache) {
      return Promise.resolve(this.styleCache);
    }
    return new Promise(resolve => {
      this.styleCallbacks = this.styleCallbacks || [];
      this.styleCallbacks.push(resolve);
      if (!this.fetchInitialStyle()) {
        console.debug("Failed to fetch style");
        //startRetryLoop();
      }
    });
  }

  private fetchInitialStyle(): boolean {
    const element: any = this.resolvedElement;
    const ancestor: any = this.resolvedAncestor;
    if (!element || !ancestor) return false;
    if (!this.styleCallbacks) return true;

    // Fetch layout
    const rect = element.getBoundingClientRect();
    // const ancestorTransform = ancestor.style.transform;
    const ancestorRect = ancestor.getBoundingClientRect();
    // console.log("ancestorTransform: ", ancestor.style, ancestorRect);
    const translateX = ancestorRect.x; // TODO
    const translateY = ancestorRect.y; // TODO
    const layout = new Rect({
      x: rect.x - translateX,
      y: rect.y - translateY,
      width: rect.width,
      height: rect.height
    });

    // Create style
    const style = new RNSharedElementStyle(layout, element.style);

    // console.debug("Style fetched: ", style);

    // Update cache
    this.styleCache = style;

    // Notify callbacks
    const callbacks = this.styleCallbacks;
    this.styleCallbacks = null;
    callbacks.forEach(callback => callback(style));
    return true;
  }

  requestContent(): Promise<RNSharedElementContent> {
    if (this.contentCache) {
      return Promise.resolve(this.contentCache);
    }
    return new Promise(resolve => {
      this.contentCallbacks = this.contentCallbacks || [];
      this.contentCallbacks.push(resolve);
      if (!this.fetchInitialContent()) {
        console.debug("Failed to fetch content");
        //startRetryLoop();
      }
    });
  }

  private fetchInitialContent(): boolean {
    const element: any = this.resolvedElement;
    if (!element) return false;
    if (!this.contentCallbacks) return true;

    // Fetch content size
    const size = RNSharedElementContent.getSize(element);
    if (!size) {
      return false;
    }

    // Create content
    const content = new RNSharedElementContent(element, size);

    // console.debug("Content fetched: ", content);

    // Update cache
    this.contentCache = content;

    // Notify callbacks
    const callbacks = this.contentCallbacks;
    this.contentCallbacks = null;
    callbacks.forEach(callback => callback(content));
    return true;
  }
}
