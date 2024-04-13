create_bits_image_internal (pixman_format_code_t format,
			    int                  width,
			    int                  height,
			    uint32_t *           bits,
			    int                  rowstride_bytes,
			    pixman_bool_t	 clear)
{
    pixman_image_t *image;

    /* must be a whole number of uint32_t's
     */
    return_val_if_fail (
	bits == NULL || (rowstride_bytes % sizeof (uint32_t)) == 0, NULL);

    return_val_if_fail (PIXMAN_FORMAT_BPP (format) >= PIXMAN_FORMAT_DEPTH (format), NULL);

    image = _pixman_image_allocate ();

    if (!image)
	return NULL;

    if (!_pixman_bits_image_init (image, format, width, height, bits,
				  rowstride_bytes / (int) sizeof (uint32_t),
				  clear))
    {
	free (image);
	return NULL;
    }

    return image;
}
