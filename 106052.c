replicate_pixel_float (bits_image_t *   bits,
		       int              x,
		       int              y,
		       int              width,
		       uint32_t *       b)
{
    argb_t color;
    argb_t *buffer = (argb_t *)b;
    argb_t *end;

    color = bits->fetch_pixel_float (bits, x, y);

    end = buffer + width;
    while (buffer < end)
	*(buffer++) = color;
}
