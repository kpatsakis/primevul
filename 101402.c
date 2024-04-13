png_build_gamma_table(png_structp png_ptr)
{
  png_debug(1, "in png_build_gamma_table");

  if (png_ptr->bit_depth <= 8)
  {
     int i;
     double g;

     if (png_ptr->screen_gamma > .000001)
        g = 1.0 / (png_ptr->gamma * png_ptr->screen_gamma);

     else
        g = 1.0;

     png_ptr->gamma_table = (png_bytep)png_malloc(png_ptr,
        (png_uint_32)256);

     for (i = 0; i < 256; i++)
     {
        png_ptr->gamma_table[i] = (png_byte)(pow((double)i / 255.0,
           g) * 255.0 + .5);
     }

#if defined(PNG_READ_BACKGROUND_SUPPORTED) || \
   defined(PNG_READ_RGB_TO_GRAY_SUPPORTED)
     if (png_ptr->transformations & ((PNG_BACKGROUND) | PNG_RGB_TO_GRAY))
     {

        g = 1.0 / (png_ptr->gamma);

        png_ptr->gamma_to_1 = (png_bytep)png_malloc(png_ptr,
           (png_uint_32)256);

        for (i = 0; i < 256; i++)
        {
           png_ptr->gamma_to_1[i] = (png_byte)(pow((double)i / 255.0,
              g) * 255.0 + .5);
        }


        png_ptr->gamma_from_1 = (png_bytep)png_malloc(png_ptr,
           (png_uint_32)256);

        if (png_ptr->screen_gamma > 0.000001)
           g = 1.0 / png_ptr->screen_gamma;

        else
           g = png_ptr->gamma;   /* Probably doing rgb_to_gray */

        for (i = 0; i < 256; i++)
        {
           png_ptr->gamma_from_1[i] = (png_byte)(pow((double)i / 255.0,
              g) * 255.0 + .5);

        }
     }
#endif /* PNG_READ_BACKGROUND_SUPPORTED || PNG_RGB_TO_GRAY_SUPPORTED */
  }
  else
  {
     double g;
     int i, j, shift, num;
     int sig_bit;
     png_uint_32 ig;

     if (png_ptr->color_type & PNG_COLOR_MASK_COLOR)
     {
        sig_bit = (int)png_ptr->sig_bit.red;

        if ((int)png_ptr->sig_bit.green > sig_bit)
           sig_bit = png_ptr->sig_bit.green;

        if ((int)png_ptr->sig_bit.blue > sig_bit)
           sig_bit = png_ptr->sig_bit.blue;
     }
     else
     {
        sig_bit = (int)png_ptr->sig_bit.gray;
     }

     if (sig_bit > 0)
        shift = 16 - sig_bit;

     else
        shift = 0;

     if (png_ptr->transformations & PNG_16_TO_8)
     {
        if (shift < (16 - PNG_MAX_GAMMA_8))
           shift = (16 - PNG_MAX_GAMMA_8);
     }

     if (shift > 8)
        shift = 8;

     if (shift < 0)
        shift = 0;

     png_ptr->gamma_shift = (png_byte)shift;

     num = (1 << (8 - shift));

     if (png_ptr->screen_gamma > .000001)
        g = 1.0 / (png_ptr->gamma * png_ptr->screen_gamma);
     else
        g = 1.0;

     png_ptr->gamma_16_table = (png_uint_16pp)png_calloc(png_ptr,
        (png_uint_32)(num * png_sizeof(png_uint_16p)));

     if (png_ptr->transformations & (PNG_16_TO_8 | PNG_BACKGROUND))
     {
        double fin, fout;
        png_uint_32 last, max;

        for (i = 0; i < num; i++)
        {
           png_ptr->gamma_16_table[i] = (png_uint_16p)png_malloc(png_ptr,
              (png_uint_32)(256 * png_sizeof(png_uint_16)));
        }

        g = 1.0 / g;
        last = 0;
        for (i = 0; i < 256; i++)
        {
           fout = ((double)i + 0.5) / 256.0;
           fin = pow(fout, g);
           max = (png_uint_32)(fin * (double)((png_uint_32)num << 8));
           while (last <= max)
           {
              png_ptr->gamma_16_table[(int)(last & (0xff >> shift))]
                 [(int)(last >> (8 - shift))] = (png_uint_16)(
                 (png_uint_16)i | ((png_uint_16)i << 8));
              last++;
           }
        }
        while (last < ((png_uint_32)num << 8))
        {
           png_ptr->gamma_16_table[(int)(last & (0xff >> shift))]
              [(int)(last >> (8 - shift))] = (png_uint_16)65535L;
           last++;
        }
     }
     else
     {
        for (i = 0; i < num; i++)
        {
           png_ptr->gamma_16_table[i] = (png_uint_16p)png_malloc(png_ptr,
              (png_uint_32)(256 * png_sizeof(png_uint_16)));

           ig = (((png_uint_32)i * (png_uint_32)png_gamma_shift[shift]) >> 4);

           for (j = 0; j < 256; j++)
           {
              png_ptr->gamma_16_table[i][j] =
                 (png_uint_16)(pow((double)(ig + ((png_uint_32)j << 8)) /
                    65535.0, g) * 65535.0 + .5);
           }
        }
     }

#if defined(PNG_READ_BACKGROUND_SUPPORTED) || \
   defined(PNG_READ_RGB_TO_GRAY_SUPPORTED)
     if (png_ptr->transformations & (PNG_BACKGROUND | PNG_RGB_TO_GRAY))
     {

        g = 1.0 / (png_ptr->gamma);

        png_ptr->gamma_16_to_1 = (png_uint_16pp)png_calloc(png_ptr,
           (png_uint_32)(num * png_sizeof(png_uint_16p )));

        for (i = 0; i < num; i++)
        {
           png_ptr->gamma_16_to_1[i] = (png_uint_16p)png_malloc(png_ptr,
              (png_uint_32)(256 * png_sizeof(png_uint_16)));

           ig = (((png_uint_32)i *
              (png_uint_32)png_gamma_shift[shift]) >> 4);
           for (j = 0; j < 256; j++)
           {
              png_ptr->gamma_16_to_1[i][j] =
                 (png_uint_16)(pow((double)(ig + ((png_uint_32)j << 8)) /
                    65535.0, g) * 65535.0 + .5);
           }
        }

        if (png_ptr->screen_gamma > 0.000001)
           g = 1.0 / png_ptr->screen_gamma;

        else
           g = png_ptr->gamma;   /* Probably doing rgb_to_gray */

        png_ptr->gamma_16_from_1 = (png_uint_16pp)png_calloc(png_ptr,
           (png_uint_32)(num * png_sizeof(png_uint_16p)));

        for (i = 0; i < num; i++)
        {
           png_ptr->gamma_16_from_1[i] = (png_uint_16p)png_malloc(png_ptr,
              (png_uint_32)(256 * png_sizeof(png_uint_16)));

           ig = (((png_uint_32)i *
              (png_uint_32)png_gamma_shift[shift]) >> 4);

           for (j = 0; j < 256; j++)
           {
              png_ptr->gamma_16_from_1[i][j] =
                 (png_uint_16)(pow((double)(ig + ((png_uint_32)j << 8)) /
                    65535.0, g) * 65535.0 + .5);
           }
        }
     }
#endif /* PNG_READ_BACKGROUND_SUPPORTED || PNG_RGB_TO_GRAY_SUPPORTED */
  }
}
