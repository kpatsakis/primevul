_pixman_image_get_scanline_generic_float (pixman_iter_t * iter,
					  const uint32_t *mask)
{
    pixman_iter_get_scanline_t fetch_32 = iter->data;
    uint32_t *buffer = iter->buffer;

    fetch_32 (iter, NULL);

    pixman_expand_to_float ((argb_t *)buffer, buffer, PIXMAN_a8r8g8b8, iter->width);

    return iter->buffer;
}
