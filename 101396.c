png_get_user_chunk_ptr(png_structp png_ptr)
{
   return (png_ptr? png_ptr->user_chunk_ptr : NULL);
}
