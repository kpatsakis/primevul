png_permit_mng_features (png_structp png_ptr, png_uint_32 mng_features)
{
   png_debug(1, "in png_permit_mng_features");

   if (png_ptr == NULL)
      return (png_uint_32)0;
   png_ptr->mng_features_permitted =
     (png_byte)(mng_features & PNG_ALL_MNG_FEATURES);
   return (png_uint_32)png_ptr->mng_features_permitted;
}
