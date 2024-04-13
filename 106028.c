_pixman_bits_image_init (pixman_image_t *     image,
                         pixman_format_code_t format,
                         int                  width,
                         int                  height,
                         uint32_t *           bits,
                         int                  rowstride,
			 pixman_bool_t	      clear)
{
    uint32_t *free_me = NULL;

    if (!bits && width && height)
    {
	int rowstride_bytes;

	free_me = bits = create_bits (format, width, height, &rowstride_bytes, clear);

	if (!bits)
	    return FALSE;

	rowstride = rowstride_bytes / (int) sizeof (uint32_t);
    }

    _pixman_image_init (image);

    image->type = BITS;
    image->bits.format = format;
    image->bits.width = width;
    image->bits.height = height;
    image->bits.bits = bits;
    image->bits.free_me = free_me;
    image->bits.read_func = NULL;
    image->bits.write_func = NULL;
    image->bits.rowstride = rowstride;
    image->bits.indexed = NULL;

    image->common.property_changed = bits_image_property_changed;

    _pixman_image_reset_clip_region (image);

    return TRUE;
}
