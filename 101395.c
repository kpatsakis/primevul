png_get_text(png_structp png_ptr, png_infop info_ptr, png_textp *text_ptr,
   int *num_text)
{
   if (png_ptr != NULL && info_ptr != NULL && info_ptr->num_text > 0)
   {
      png_debug1(1, "in %s retrieval function",
         (png_ptr->chunk_name[0] == '\0' ? "text"
             : (png_const_charp)png_ptr->chunk_name));

      if (text_ptr != NULL)
         *text_ptr = info_ptr->text;

      if (num_text != NULL)
         *num_text = info_ptr->num_text;

      return ((png_uint_32)info_ptr->num_text);
   }
   if (num_text != NULL)
     *num_text = 0;
   return(0);
}
