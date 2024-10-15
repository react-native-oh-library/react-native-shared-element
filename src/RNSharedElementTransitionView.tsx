import { requireNativeComponent, NativeModules, Platform } from "react-native";

let RNSharedElementTransitionView;
const isAvailable = !!NativeModules.RNSharedElementTransition;

// @ts-ignore
if (Platform.OS === "harmony") {
    RNSharedElementTransitionView = requireNativeComponent("RNSharedElementTransition");
} else {
    if (isAvailable) {
        NativeModules.RNSharedElementTransition.configure({
            imageResolvers: [
                "RNPhotoView.MWTapDetectingImageView", // react-native-photo-view
            ].map(path => path.split(".")),
        });
    }
    RNSharedElementTransitionView = isAvailable
        ? requireNativeComponent("RNSharedElementTransition")
        : undefined;
}

export { RNSharedElementTransitionView };