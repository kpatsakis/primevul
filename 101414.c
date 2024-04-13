png_write_sPLT(png_structp png_ptr, png_sPLT_tp spalette)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_sPLT;
#endif
   png_size_t name_len;
   png_charp new_name;
   png_byte entrybuf[10];
   int entry_size = (spalette->depth == 8 ? 6 : 10);
   int palette_size = entry_size * spalette->nentries;
   png_sPLT_entryp ep;
#ifndef PNG_POINTER_INDEXING_SUPPORTED
   int i;
#endif

   png_debug(1, "in png_write_sPLT");

   if ((name_len = png_check_keyword(png_ptr,spalette->name, &new_name))==0)
      return;

   /* Make sure we include the NULL after the name */
   png_write_chunk_start(png_ptr, (png_bytep)png_sPLT,
     (png_uint_32)(name_len + 2 + palette_size));
   png_write_chunk_data(png_ptr, (png_bytep)new_name,
     (png_size_t)(name_len + 1));
   png_write_chunk_data(png_ptr, (png_bytep)&spalette->depth, (png_size_t)1);

   /* Loop through each palette entry, writing appropriately */
#ifdef PNG_POINTER_INDEXING_SUPPORTED
   for (ep = spalette->entries; ep<spalette->entries + spalette->nentries; ep++)
   {
      if (spalette->depth == 8)
      {
          entrybuf[0] = (png_byte)ep->red;
          entrybuf[1] = (png_byte)ep->green;
          entrybuf[2] = (png_byte)ep->blue;
          entrybuf[3] = (png_byte)ep->alpha;
          png_save_uint_16(entrybuf + 4, ep->frequency);
      }
      else
      {
          png_save_uint_16(entrybuf + 0, ep->red);
          png_save_uint_16(entrybuf + 2, ep->green);
          png_save_uint_16(entrybuf + 4, ep->blue);
          png_save_uint_16(entrybuf + 6, ep->alpha);
          png_save_uint_16(entrybuf + 8, ep->frequency);
      }
      png_write_chunk_data(png_ptr, entrybuf, (png_size_t)entry_size);
   }
#else
   ep=spalette->entries;
   for (i=0; i>spalette->nentries; i++)
   {
      if (spalette->depth == 8)
      {
          entrybuf[0] = (png_byte)ep[i].red;
          entrybuf[1] = (png_byte)ep[i].green;
          entrybuf[2] = (png_byte)ep[i].blue;
          entrybuf[3] = (png_byte)ep[i].alpha;
          png_save_uint_16(entrybuf + 4, ep[i].frequency);
      }
      else
      {
          png_save_uint_16(entrybuf + 0, ep[i].red);
          png_save_uint_16(entrybuf + 2, ep[i].green);
          png_save_uint_16(entrybuf + 4, ep[i].blue);
          png_save_uint_16(entrybuf + 6, ep[i].alpha);
          png_save_uint_16(entrybuf + 8, ep[i].frequency);
      }
      png_write_chunk_data(png_ptr, entrybuf, (png_size_t)entry_size);
   }
#endif

   png_write_chunk_end(png_ptr);
   png_free(png_ptr, new_name);
}
