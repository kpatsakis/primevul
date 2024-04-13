pixman_image_create_bits_no_clear (pixman_format_code_t format,
				   int                  width,
				   int                  height,
				   uint32_t *           bits,
				   int                  rowstride_bytes)
{
    return create_bits_image_internal (
	format, width, height, bits, rowstride_bytes, FALSE);
}
