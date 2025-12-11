/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */

import {
  RNOHPackage, TurboModulesFactory } from '@rnoh/react-native-openharmony';
import type { TurboModule, TurboModuleContext } from '@rnoh/react-native-openharmony/ts';
import { SharedElementModule } from './SharedElementModule';

class RNSharedElementTurboModulesFactory extends TurboModulesFactory {
  createTurboModule(name: string): TurboModule | null {
    if (name === 'RNSharedElementTransition') {
      return new SharedElementModule(this.ctx);
    }
    return null;
  }

  hasTurboModule(name: string): boolean {
    return name === "RNSharedElementTransition"
  }
}

export class SharedElementPackage extends RNOHPackage  {
  createTurboModulesFactory(ctx: TurboModuleContext): TurboModulesFactory {
    return new RNSharedElementTurboModulesFactory(ctx);
  }
}