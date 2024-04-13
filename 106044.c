dest_get_scanline_wide (pixman_iter_t *iter, const uint32_t *mask)
{
    bits_image_t *  image  = &iter->image->bits;
    int             x      = iter->x;
    int             y      = iter->y;
    int             width  = iter->width;
    argb_t *	    buffer = (argb_t *)iter->buffer;

    image->fetch_scanline_float (
	image, x, y, width, (uint32_t *)buffer, mask);
    if (image->common.alpha_map)
    {
	argb_t *alpha;

	if ((alpha = malloc (width * sizeof (argb_t))))
	{
	    int i;

	    x -= image->common.alpha_origin_x;
	    y -= image->common.alpha_origin_y;

	    image->common.alpha_map->fetch_scanline_float (
		image->common.alpha_map, x, y, width, (uint32_t *)alpha, mask);

	    for (i = 0; i < width; ++i)
		buffer[i].a = alpha[i].a;

	    free (alpha);
	}
    }

    return iter->buffer;
}
