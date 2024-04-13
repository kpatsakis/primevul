_pixman_bits_image_dest_iter_init (pixman_image_t *image, pixman_iter_t *iter)
{
    if (iter->iter_flags & ITER_NARROW)
    {
	if ((iter->iter_flags & (ITER_IGNORE_RGB | ITER_IGNORE_ALPHA)) ==
	    (ITER_IGNORE_RGB | ITER_IGNORE_ALPHA))
	{
	    iter->get_scanline = _pixman_iter_get_scanline_noop;
	}
	else
	{
	    iter->get_scanline = dest_get_scanline_narrow;
	}
	
	iter->write_back = dest_write_back_narrow;
    }
    else
    {
	iter->get_scanline = dest_get_scanline_wide;
	iter->write_back = dest_write_back_wide;
    }
}
