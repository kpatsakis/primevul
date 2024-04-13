bits_image_property_changed (pixman_image_t *image)
{
    _pixman_bits_image_setup_accessors (&image->bits);
}
