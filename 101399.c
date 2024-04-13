png_progressive_combine_row (png_structp png_ptr,
   png_bytep old_row, png_bytep new_row)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_CONST int FARDATA png_pass_dsp_mask[7] =
      {0xff, 0x0f, 0xff, 0x33, 0xff, 0x55, 0xff};
#endif

   if (png_ptr == NULL)
      return;

   if (new_row != NULL)    /* new_row must == png_ptr->row_buf here. */
      png_combine_row(png_ptr, old_row, png_pass_dsp_mask[png_ptr->pass]);
}
