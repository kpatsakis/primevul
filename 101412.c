png_write_cHRM(png_structp png_ptr, double white_x, double white_y,
   double red_x, double red_y, double green_x, double green_y,
   double blue_x, double blue_y)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_cHRM;
#endif
   png_byte buf[32];

   png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y,
      int_green_x, int_green_y, int_blue_x, int_blue_y;

   png_debug(1, "in png_write_cHRM");

   int_white_x = (png_uint_32)(white_x * 100000.0 + 0.5);
   int_white_y = (png_uint_32)(white_y * 100000.0 + 0.5);
   int_red_x   = (png_uint_32)(red_x   * 100000.0 + 0.5);
   int_red_y   = (png_uint_32)(red_y   * 100000.0 + 0.5);
   int_green_x = (png_uint_32)(green_x * 100000.0 + 0.5);
   int_green_y = (png_uint_32)(green_y * 100000.0 + 0.5);
   int_blue_x  = (png_uint_32)(blue_x  * 100000.0 + 0.5);
   int_blue_y  = (png_uint_32)(blue_y  * 100000.0 + 0.5);

#ifdef PNG_CHECK_cHRM_SUPPORTED
   if (png_check_cHRM_fixed(png_ptr, int_white_x, int_white_y,
      int_red_x, int_red_y, int_green_x, int_green_y, int_blue_x, int_blue_y))
#endif
   {
      /* Each value is saved in 1/100,000ths */

      png_save_uint_32(buf, int_white_x);
      png_save_uint_32(buf + 4, int_white_y);

      png_save_uint_32(buf + 8, int_red_x);
      png_save_uint_32(buf + 12, int_red_y);

      png_save_uint_32(buf + 16, int_green_x);
      png_save_uint_32(buf + 20, int_green_y);

      png_save_uint_32(buf + 24, int_blue_x);
      png_save_uint_32(buf + 28, int_blue_y);

      png_write_chunk(png_ptr, (png_bytep)png_cHRM, buf, (png_size_t)32);
   }
}
