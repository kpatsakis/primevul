static void vnc_write_pixels_copy(VncState *vs,
                                  void *pixels, int size)
{
    vnc_write(vs, pixels, size);
}
