bits_image_fetch_untransformed_repeat_none (bits_image_t *image,
                                            pixman_bool_t wide,
                                            int           x,
                                            int           y,
                                            int           width,
                                            uint32_t *    buffer)
{
    uint32_t w;

    if (y < 0 || y >= image->height)
    {
	memset (buffer, 0, width * (wide? sizeof (argb_t) : 4));
	return;
    }

    if (x < 0)
    {
	w = MIN (width, -x);

	memset (buffer, 0, w * (wide ? sizeof (argb_t) : 4));

	width -= w;
	buffer += w * (wide? 4 : 1);
	x += w;
    }

    if (x < image->width)
    {
	w = MIN (width, image->width - x);

	if (wide)
	    image->fetch_scanline_float (image, x, y, w, buffer, NULL);
	else
	    image->fetch_scanline_32 (image, x, y, w, buffer, NULL);

	width -= w;
	buffer += w * (wide? 4 : 1);
	x += w;
    }

    memset (buffer, 0, width * (wide ? sizeof (argb_t) : 4));
}
