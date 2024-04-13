void Splash::compositeBackground(SplashColorPtr color) {
  SplashColorPtr p;
  Guchar *q;
  Guchar alpha, alpha1, c, color0, color1, color2;
#if SPLASH_CMYK
  Guchar color3;
  Guchar colorsp[SPOT_NCOMPS+4], cp;
#endif
  int x, y, mask;

  if (unlikely(bitmap->alpha == NULL)) {
    error(errInternal, -1, "bitmap->alpha is NULL in Splash::compositeBackground");
    return;
  }

  switch (bitmap->mode) {
  case splashModeMono1:
    color0 = color[0];
    for (y = 0; y < bitmap->height; ++y) {
      p = &bitmap->data[y * bitmap->rowSize];
      q = &bitmap->alpha[y * bitmap->width];
      mask = 0x80;
      for (x = 0; x < bitmap->width; ++x) {
	alpha = *q++;
	alpha1 = 255 - alpha;
	c = (*p & mask) ? 0xff : 0x00;
	c = div255(alpha1 * color0 + alpha * c);
	if (c & 0x80) {
	  *p |= mask;
	} else {
	  *p &= ~mask;
	}
	if (!(mask >>= 1)) {
	  mask = 0x80;
	  ++p;
	}
      }
    }
    break;
  case splashModeMono8:
    color0 = color[0];
    for (y = 0; y < bitmap->height; ++y) {
      p = &bitmap->data[y * bitmap->rowSize];
      q = &bitmap->alpha[y * bitmap->width];
      for (x = 0; x < bitmap->width; ++x) {
	alpha = *q++;
	alpha1 = 255 - alpha;
	p[0] = div255(alpha1 * color0 + alpha * p[0]);
	++p;
      }
    }
    break;
  case splashModeRGB8:
  case splashModeBGR8:
    color0 = color[0];
    color1 = color[1];
    color2 = color[2];
    for (y = 0; y < bitmap->height; ++y) {
      p = &bitmap->data[y * bitmap->rowSize];
      q = &bitmap->alpha[y * bitmap->width];
      for (x = 0; x < bitmap->width; ++x) {
	alpha = *q++;
	if (alpha == 0)
	{
	  p[0] = color0;
	  p[1] = color1;
	  p[2] = color2;
	}
	else if (alpha != 255)
	{
	  alpha1 = 255 - alpha;
	  p[0] = div255(alpha1 * color0 + alpha * p[0]);
	  p[1] = div255(alpha1 * color1 + alpha * p[1]);
	  p[2] = div255(alpha1 * color2 + alpha * p[2]);
	}
	p += 3;
      }
    }
    break;
  case splashModeXBGR8:
    color0 = color[0];
    color1 = color[1];
    color2 = color[2];
    for (y = 0; y < bitmap->height; ++y) {
      p = &bitmap->data[y * bitmap->rowSize];
      q = &bitmap->alpha[y * bitmap->width];
      for (x = 0; x < bitmap->width; ++x) {
	alpha = *q++;
	if (alpha == 0)
	{
	  p[0] = color0;
	  p[1] = color1;
	  p[2] = color2;
	}
	else if (alpha != 255)
	{
	  alpha1 = 255 - alpha;
	  p[0] = div255(alpha1 * color0 + alpha * p[0]);
	  p[1] = div255(alpha1 * color1 + alpha * p[1]);
	  p[2] = div255(alpha1 * color2 + alpha * p[2]);
	}
	p[3] = 255;
	p += 4;
      }
    }
    break;
#if SPLASH_CMYK
  case splashModeCMYK8:
    color0 = color[0];
    color1 = color[1];
    color2 = color[2];
    color3 = color[3];
    for (y = 0; y < bitmap->height; ++y) {
      p = &bitmap->data[y * bitmap->rowSize];
      q = &bitmap->alpha[y * bitmap->width];
      for (x = 0; x < bitmap->width; ++x) {
	alpha = *q++;
	if (alpha == 0)
	{
	  p[0] = color0;
	  p[1] = color1;
	  p[2] = color2;
	  p[3] = color3;
	}
	else if (alpha != 255)
	{
	  alpha1 = 255 - alpha;
	  p[0] = div255(alpha1 * color0 + alpha * p[0]);
	  p[1] = div255(alpha1 * color1 + alpha * p[1]);
	  p[2] = div255(alpha1 * color2 + alpha * p[2]);
    p[3] = div255(alpha1 * color3 + alpha * p[3]);
	}
	p += 4;
      }
    }
    break;
  case splashModeDeviceN8:
    for (cp = 0; cp < SPOT_NCOMPS+4; cp++)
      colorsp[cp] = color[cp];
    for (y = 0; y < bitmap->height; ++y) {
      p = &bitmap->data[y * bitmap->rowSize];
      q = &bitmap->alpha[y * bitmap->width];
      for (x = 0; x < bitmap->width; ++x) {
	alpha = *q++;
	if (alpha == 0)
	{
    for (cp = 0; cp < SPOT_NCOMPS+4; cp++)
      p[cp] = colorsp[cp];
	}
	else if (alpha != 255)
	{
	  alpha1 = 255 - alpha;
    for (cp = 0; cp < SPOT_NCOMPS+4; cp++)
      p[cp] = div255(alpha1 * colorsp[cp] + alpha * p[cp]);
	}
	p += (SPOT_NCOMPS+4);
      }
    }
    break;
#endif
  }
  memset(bitmap->alpha, 255, bitmap->width * bitmap->height);
}
