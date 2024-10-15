import { TurboModule, TurboModuleContext } from '@rnoh/react-native-openharmony/ts';
import { componentSnapshot } from '@kit.ArkUI';

type Options = {
  width?: number,
  height?: number,
  stride: number,
  pixelFormat: number,
  alphaType: number,
};

export class SharedElementModule extends TurboModule {
  public static readonly NAME = 'RNSharedElementTransition';

  constructor(protected ctx: TurboModuleContext) {
    super(ctx);
    console.log('ArkTS side------>SharedElementModule constructor');
  }

  options: Options | null = null;

  public configure(): boolean {
    console.log('ArkTS side------>configure');
    return true;
  }

  public getSnap(tag: number): boolean {
    console.log('ArkTS side------>getSnap tag', tag);
    try {
      let pixelmap = componentSnapshot.getSync(tag+'');
      let originImageInfo = pixelmap.getImageInfoSync();
      let dstX = originImageInfo.size.width;
      let dstY = originImageInfo.size.height;
      this.options = {
        width: dstX,
        height: dstY,
        stride: originImageInfo.stride,
        pixelFormat: originImageInfo.pixelFormat,
        alphaType: originImageInfo.alphaType,
      };
      console.log('ArkTS side------>getSnap result pixelmap', pixelmap);
      console.log('ArkTS side------>getSnap result originImageInfo', originImageInfo);
      console.log('ArkTS side------>getSnap result dstX', dstX);
      console.log('ArkTS side------>getSnap result dstY', dstY);
      console.log('ArkTS side------>getSnap result options', this.options);

      //testNapi.TagGetView_s(dstX,dstY,originImageInfo.stride,originImageInfo.pixelFormat,originImageInfo.alphaType,pixelmap);
    } catch (error) {
      console.error("Error occurred while calling getSync:", error);
      return false;
    }
    return true;
  }
}

