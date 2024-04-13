_pixman_bits_image_src_iter_init (pixman_image_t *image, pixman_iter_t *iter)
{
    pixman_format_code_t format = image->common.extended_format_code;
    uint32_t flags = image->common.flags;
    const fetcher_info_t *info;

    for (info = fetcher_info; info->format != PIXMAN_null; ++info)
    {
	if ((info->format == format || info->format == PIXMAN_any)	&&
	    (info->flags & flags) == info->flags)
	{
	    if (iter->iter_flags & ITER_NARROW)
	    {
		iter->get_scanline = info->get_scanline_32;
	    }
	    else
	    {
		iter->data = info->get_scanline_32;
		iter->get_scanline = info->get_scanline_float;
	    }
	    return;
	}
    }

    /* Just in case we somehow didn't find a scanline function */
    iter->get_scanline = _pixman_iter_get_scanline_noop;
}
