dest_get_scanline_narrow (pixman_iter_t *iter, const uint32_t *mask)
{
    pixman_image_t *image  = iter->image;
    int             x      = iter->x;
    int             y      = iter->y;
    int             width  = iter->width;
    uint32_t *	    buffer = iter->buffer;

    image->bits.fetch_scanline_32 (&image->bits, x, y, width, buffer, mask);
    if (image->common.alpha_map)
    {
	uint32_t *alpha;

	if ((alpha = malloc (width * sizeof (uint32_t))))
	{
	    int i;

	    x -= image->common.alpha_origin_x;
	    y -= image->common.alpha_origin_y;

	    image->common.alpha_map->fetch_scanline_32 (
		image->common.alpha_map, x, y, width, alpha, mask);

	    for (i = 0; i < width; ++i)
	    {
		buffer[i] &= ~0xff000000;
		buffer[i] |= (alpha[i] & 0xff000000);
	    }

	    free (alpha);
	}
    }

    return iter->buffer;
}
