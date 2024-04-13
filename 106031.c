bits_image_fetch_affine_no_alpha (pixman_iter_t *  iter,
				  const uint32_t * mask)
{
    pixman_image_t *image  = iter->image;
    int             offset = iter->x;
    int             line   = iter->y++;
    int             width  = iter->width;
    uint32_t *      buffer = iter->buffer;

    pixman_fixed_t x, y;
    pixman_fixed_t ux, uy;
    pixman_vector_t v;
    int i;

    /* reference point is the center of the pixel */
    v.vector[0] = pixman_int_to_fixed (offset) + pixman_fixed_1 / 2;
    v.vector[1] = pixman_int_to_fixed (line) + pixman_fixed_1 / 2;
    v.vector[2] = pixman_fixed_1;

    if (image->common.transform)
    {
	if (!pixman_transform_point_3d (image->common.transform, &v))
	    return iter->buffer;

	ux = image->common.transform->matrix[0][0];
	uy = image->common.transform->matrix[1][0];
    }
    else
    {
	ux = pixman_fixed_1;
	uy = 0;
    }

    x = v.vector[0];
    y = v.vector[1];

    for (i = 0; i < width; ++i)
    {
	if (!mask || mask[i])
	{
	    buffer[i] = bits_image_fetch_pixel_filtered (
		&image->bits, x, y, fetch_pixel_no_alpha);
	}

	x += ux;
	y += uy;
    }

    return buffer;
}
