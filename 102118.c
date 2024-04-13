static void FromColor_D565(void* dst, const SkColor src[], int width,
 int x, int y) {
 uint16_t* d = (uint16_t*)dst;

    DITHER_565_SCAN(y);
 for (int stop = x + width; x < stop; x++) {
 SkColor c = *src++;
 *d++ = SkDitherRGBTo565(SkColorGetR(c), SkColorGetG(c), SkColorGetB(c),
                                DITHER_VALUE(x));
 }
}
