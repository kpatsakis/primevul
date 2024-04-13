png_convert_size(size_t size)
{
   if (size > (png_size_t)-1)
      PNG_ABORT();  /* We haven't got access to png_ptr, so no png_error() */
   return ((png_size_t)size);
}
