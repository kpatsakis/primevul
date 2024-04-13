bits_image_fetch_pixel_separable_convolution (bits_image_t *image,
                                              pixman_fixed_t x,
                                              pixman_fixed_t y,
                                              get_pixel_t    get_pixel)
{
    pixman_fixed_t *params = image->common.filter_params;
    pixman_repeat_t repeat_mode = image->common.repeat;
    int width = image->width;
    int height = image->height;
    int cwidth = pixman_fixed_to_int (params[0]);
    int cheight = pixman_fixed_to_int (params[1]);
    int x_phase_bits = pixman_fixed_to_int (params[2]);
    int y_phase_bits = pixman_fixed_to_int (params[3]);
    int x_phase_shift = 16 - x_phase_bits;
    int y_phase_shift = 16 - y_phase_bits;
    int x_off = ((cwidth << 16) - pixman_fixed_1) >> 1;
    int y_off = ((cheight << 16) - pixman_fixed_1) >> 1;
    pixman_fixed_t *y_params;
    int srtot, sgtot, sbtot, satot;
    int32_t x1, x2, y1, y2;
    int32_t px, py;
    int i, j;

    /* Round x and y to the middle of the closest phase before continuing. This
     * ensures that the convolution matrix is aligned right, since it was
     * positioned relative to a particular phase (and not relative to whatever
     * exact fraction we happen to get here).
     */
    x = ((x >> x_phase_shift) << x_phase_shift) + ((1 << x_phase_shift) >> 1);
    y = ((y >> y_phase_shift) << y_phase_shift) + ((1 << y_phase_shift) >> 1);

    px = (x & 0xffff) >> x_phase_shift;
    py = (y & 0xffff) >> y_phase_shift;

    y_params = params + 4 + (1 << x_phase_bits) * cwidth + py * cheight;

    x1 = pixman_fixed_to_int (x - pixman_fixed_e - x_off);
    y1 = pixman_fixed_to_int (y - pixman_fixed_e - y_off);
    x2 = x1 + cwidth;
    y2 = y1 + cheight;

    srtot = sgtot = sbtot = satot = 0;

    for (i = y1; i < y2; ++i)
    {
        pixman_fixed_48_16_t fy = *y_params++;
        pixman_fixed_t *x_params = params + 4 + px * cwidth;

        if (fy)
        {
            for (j = x1; j < x2; ++j)
            {
                pixman_fixed_t fx = *x_params++;
		int rx = j;
		int ry = i;

                if (fx)
                {
                    pixman_fixed_t f;
                    uint32_t pixel;

                    if (repeat_mode != PIXMAN_REPEAT_NONE)
                    {
                        repeat (repeat_mode, &rx, width);
                        repeat (repeat_mode, &ry, height);

                        pixel = get_pixel (image, rx, ry, FALSE);
                    }
                    else
                    {
                        pixel = get_pixel (image, rx, ry, TRUE);
		    }

                    f = (fy * fx + 0x8000) >> 16;

                    srtot += (int)RED_8 (pixel) * f;
                    sgtot += (int)GREEN_8 (pixel) * f;
                    sbtot += (int)BLUE_8 (pixel) * f;
                    satot += (int)ALPHA_8 (pixel) * f;
                }
            }
	}
    }

    satot = (satot + 0x8000) >> 16;
    srtot = (srtot + 0x8000) >> 16;
    sgtot = (sgtot + 0x8000) >> 16;
    sbtot = (sbtot + 0x8000) >> 16;

    satot = CLIP (satot, 0, 0xff);
    srtot = CLIP (srtot, 0, 0xff);
    sgtot = CLIP (sgtot, 0, 0xff);
    sbtot = CLIP (sbtot, 0, 0xff);

    return ((satot << 24) | (srtot << 16) | (sgtot <<  8) | (sbtot));
}
