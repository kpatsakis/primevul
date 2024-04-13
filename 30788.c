inline void Splash::pipeInit(SplashPipe *pipe, int x, int y,
			     SplashPattern *pattern, SplashColorPtr cSrc,
			     Guchar aInput, GBool usesShape,
			     GBool nonIsolatedGroup,
			     GBool knockout, Guchar knockoutOpacity) {
  pipeSetXY(pipe, x, y);
  pipe->pattern = NULL;

  if (pattern) {
    if (pattern->isStatic()) {
      pattern->getColor(x, y, pipe->cSrcVal);
    } else {
      pipe->pattern = pattern;
    }
    pipe->cSrc = pipe->cSrcVal;
  } else {
    pipe->cSrc = cSrc;
  }

  pipe->aInput = aInput;
  pipe->usesShape = usesShape;

  pipe->knockout = knockout;
  pipe->knockoutOpacity = knockoutOpacity;

  if (aInput == 255 && !state->softMask && !usesShape &&
      !state->inNonIsolatedGroup && !nonIsolatedGroup) {
    pipe->noTransparency = gTrue;
  } else {
    pipe->noTransparency = gFalse;
  }

  if (pipe->noTransparency) {
    pipe->resultColorCtrl = pipeResultColorNoAlphaBlend[bitmap->mode];
  } else if (!state->blendFunc) {
    pipe->resultColorCtrl = pipeResultColorAlphaNoBlend[bitmap->mode];
  } else {
    pipe->resultColorCtrl = pipeResultColorAlphaBlend[bitmap->mode];
  }

  pipe->nonIsolatedGroup = nonIsolatedGroup;

  pipe->run = &Splash::pipeRun;
  if (!pipe->pattern && pipe->noTransparency && !state->blendFunc) {
    if (bitmap->mode == splashModeMono1 && !pipe->destAlphaPtr) {
      pipe->run = &Splash::pipeRunSimpleMono1;
    } else if (bitmap->mode == splashModeMono8 && pipe->destAlphaPtr) {
      pipe->run = &Splash::pipeRunSimpleMono8;
    } else if (bitmap->mode == splashModeRGB8 && pipe->destAlphaPtr) {
      pipe->run = &Splash::pipeRunSimpleRGB8;
    } else if (bitmap->mode == splashModeXBGR8 && pipe->destAlphaPtr) {
      pipe->run = &Splash::pipeRunSimpleXBGR8;
    } else if (bitmap->mode == splashModeBGR8 && pipe->destAlphaPtr) {
      pipe->run = &Splash::pipeRunSimpleBGR8;
#if SPLASH_CMYK
    } else if (bitmap->mode == splashModeCMYK8 && pipe->destAlphaPtr) {
      pipe->run = &Splash::pipeRunSimpleCMYK8;
    } else if (bitmap->mode == splashModeDeviceN8 && pipe->destAlphaPtr) {
      pipe->run = &Splash::pipeRunSimpleDeviceN8;
#endif
    }
  } else if (!pipe->pattern && !pipe->noTransparency && !state->softMask &&
	     pipe->usesShape &&
	     !(state->inNonIsolatedGroup && alpha0Bitmap->alpha) &&
	     !state->blendFunc && !pipe->nonIsolatedGroup) {
    if (bitmap->mode == splashModeMono1 && !pipe->destAlphaPtr) {
      pipe->run = &Splash::pipeRunAAMono1;
    } else if (bitmap->mode == splashModeMono8 && pipe->destAlphaPtr) {
      pipe->run = &Splash::pipeRunAAMono8;
    } else if (bitmap->mode == splashModeRGB8 && pipe->destAlphaPtr) {
      pipe->run = &Splash::pipeRunAARGB8;
    } else if (bitmap->mode == splashModeXBGR8 && pipe->destAlphaPtr) {
      pipe->run = &Splash::pipeRunAAXBGR8;
    } else if (bitmap->mode == splashModeBGR8 && pipe->destAlphaPtr) {
      pipe->run = &Splash::pipeRunAABGR8;
#if SPLASH_CMYK
    } else if (bitmap->mode == splashModeCMYK8 && pipe->destAlphaPtr) {
      pipe->run = &Splash::pipeRunAACMYK8;
    } else if (bitmap->mode == splashModeDeviceN8 && pipe->destAlphaPtr) {
      pipe->run = &Splash::pipeRunAADeviceN8;
#endif
    }
  }
}
