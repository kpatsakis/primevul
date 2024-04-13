png_get_x_offset_pixels(png_structp png_ptr, png_infop info_ptr)
{
   if (png_ptr != NULL && info_ptr != NULL)

#ifdef PNG_oFFs_SUPPORTED
   if (info_ptr->valid & PNG_INFO_oFFs)
   {
      png_debug1(1, "in %s retrieval function", "png_get_x_offset_microns");

      if (info_ptr->offset_unit_type != PNG_OFFSET_PIXEL)
          return (0);

      else
          return (info_ptr->x_offset);
   }
#else
   return (0);
#endif
   return (0);
}
