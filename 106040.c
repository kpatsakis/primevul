bits_image_fetch_untransformed_repeat_normal (bits_image_t *image,
                                              pixman_bool_t wide,
                                              int           x,
                                              int           y,
                                              int           width,
                                              uint32_t *    buffer)
{
    uint32_t w;

    while (y < 0)
	y += image->height;

    while (y >= image->height)
	y -= image->height;

    if (image->width == 1)
    {
	if (wide)
	    replicate_pixel_float (image, 0, y, width, buffer);
	else
	    replicate_pixel_32 (image, 0, y, width, buffer);

	return;
    }

    while (width)
    {
	while (x < 0)
	    x += image->width;
	while (x >= image->width)
	    x -= image->width;

	w = MIN (width, image->width - x);

	if (wide)
	    image->fetch_scanline_float (image, x, y, w, buffer, NULL);
	else
	    image->fetch_scanline_32 (image, x, y, w, buffer, NULL);

	buffer += w * (wide? 4 : 1);
	x += w;
	width -= w;
    }
}
