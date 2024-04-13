png_check_cHRM_fixed(png_structp png_ptr,
   png_fixed_point white_x, png_fixed_point white_y, png_fixed_point red_x,
   png_fixed_point red_y, png_fixed_point green_x, png_fixed_point green_y,
   png_fixed_point blue_x, png_fixed_point blue_y)
{
   int ret = 1;
   unsigned long xy_hi,xy_lo,yx_hi,yx_lo;

   png_debug(1, "in function png_check_cHRM_fixed");

   if (png_ptr == NULL)
      return 0;

   if (white_x < 0 || white_y <= 0 ||
         red_x < 0 ||   red_y <  0 ||
       green_x < 0 || green_y <  0 ||
        blue_x < 0 ||  blue_y <  0)
   {
      png_warning(png_ptr,
        "Ignoring attempt to set negative chromaticity value");
      ret = 0;
   }
   if (white_x > (png_fixed_point) PNG_UINT_31_MAX ||
       white_y > (png_fixed_point) PNG_UINT_31_MAX ||
         red_x > (png_fixed_point) PNG_UINT_31_MAX ||
         red_y > (png_fixed_point) PNG_UINT_31_MAX ||
       green_x > (png_fixed_point) PNG_UINT_31_MAX ||
       green_y > (png_fixed_point) PNG_UINT_31_MAX ||
        blue_x > (png_fixed_point) PNG_UINT_31_MAX ||
        blue_y > (png_fixed_point) PNG_UINT_31_MAX )
   {
      png_warning(png_ptr,
        "Ignoring attempt to set chromaticity value exceeding 21474.83");
      ret = 0;
   }
   if (white_x > 100000L - white_y)
   {
      png_warning(png_ptr, "Invalid cHRM white point");
      ret = 0;
   }
   if (red_x > 100000L - red_y)
   {
      png_warning(png_ptr, "Invalid cHRM red point");
      ret = 0;
   }
   if (green_x > 100000L - green_y)
   {
      png_warning(png_ptr, "Invalid cHRM green point");
      ret = 0;
   }
   if (blue_x > 100000L - blue_y)
   {
      png_warning(png_ptr, "Invalid cHRM blue point");
      ret = 0;
   }

   png_64bit_product(green_x - red_x, blue_y - red_y, &xy_hi, &xy_lo);
   png_64bit_product(green_y - red_y, blue_x - red_x, &yx_hi, &yx_lo);

   if (xy_hi == yx_hi && xy_lo == yx_lo)
   {
      png_warning(png_ptr,
         "Ignoring attempt to set cHRM RGB triangle with zero area");
      ret = 0;
   }

   return ret;
}
