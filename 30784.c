static GBool isImageInterpolationRequired(int srcWidth, int srcHeight,
                                          int scaledWidth, int scaledHeight,
                                          GBool interpolate) {
  if (interpolate)
    return gTrue;

  /* When scale factor is >= 400% we don't interpolate. See bugs #25268, #9860 */
  if (scaledWidth / srcWidth >= 4 || scaledHeight / srcHeight >= 4)
    return gFalse;

  return gTrue;
}
