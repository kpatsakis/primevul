static void FromColor_D32(void* dst, const SkColor src[], int width,
 int, int) {
 SkPMColor* d = (SkPMColor*)dst;

 for (int i = 0; i < width; i++) {
 *d++ = SkPreMultiplyColor(*src++);
 }
}
