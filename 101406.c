png_set_read_user_transform_fn(png_structp png_ptr, png_user_transform_ptr
   read_user_transform_fn)
{
   png_debug(1, "in png_set_read_user_transform_fn");

   if (png_ptr == NULL)
      return;

#ifdef PNG_READ_USER_TRANSFORM_SUPPORTED
   png_ptr->transformations |= PNG_USER_TRANSFORM;
   png_ptr->read_user_transform_fn = read_user_transform_fn;
#endif
#ifdef PNG_LEGACY_SUPPORTED
   if (read_user_transform_fn)
      png_warning(png_ptr,
        "This version of libpng does not support user transforms");
#endif
}
