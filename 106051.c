replicate_pixel_32 (bits_image_t *   bits,
		    int              x,
		    int              y,
		    int              width,
		    uint32_t *       buffer)
{
    uint32_t color;
    uint32_t *end;

    color = bits->fetch_pixel_32 (bits, x, y);

    end = buffer + width;
    while (buffer < end)
	*(buffer++) = color;
}
