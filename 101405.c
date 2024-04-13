png_set_dither(png_structp png_ptr, png_colorp palette,
   int num_palette, int maximum_colors, png_uint_16p histogram,
   int full_dither)
{
   png_debug(1, "in png_set_dither");

   if (png_ptr == NULL)
      return;
   png_ptr->transformations |= PNG_DITHER;

   if (!full_dither)
   {
      int i;

      png_ptr->dither_index = (png_bytep)png_malloc(png_ptr,
         (png_uint_32)(num_palette * png_sizeof(png_byte)));
      for (i = 0; i < num_palette; i++)
         png_ptr->dither_index[i] = (png_byte)i;
   }

   if (num_palette > maximum_colors)
   {
      if (histogram != NULL)
      {
         /* This is easy enough, just throw out the least used colors.
          * Perhaps not the best solution, but good enough.
          */

         int i;

         /* Initialize an array to sort colors */
         png_ptr->dither_sort = (png_bytep)png_malloc(png_ptr,
            (png_uint_32)(num_palette * png_sizeof(png_byte)));

         /* Initialize the dither_sort array */
         for (i = 0; i < num_palette; i++)
            png_ptr->dither_sort[i] = (png_byte)i;

         /* Find the least used palette entries by starting a
          * bubble sort, and running it until we have sorted
          * out enough colors.  Note that we don't care about
          * sorting all the colors, just finding which are
          * least used.
          */

         for (i = num_palette - 1; i >= maximum_colors; i--)
         {
            int done; /* To stop early if the list is pre-sorted */
            int j;

            done = 1;
            for (j = 0; j < i; j++)
            {
               if (histogram[png_ptr->dither_sort[j]]
                   < histogram[png_ptr->dither_sort[j + 1]])
               {
                  png_byte t;

                  t = png_ptr->dither_sort[j];
                  png_ptr->dither_sort[j] = png_ptr->dither_sort[j + 1];
                  png_ptr->dither_sort[j + 1] = t;
                  done = 0;
               }
            }
            if (done)
               break;
         }

         /* Swap the palette around, and set up a table, if necessary */
         if (full_dither)
         {
            int j = num_palette;

            /* Put all the useful colors within the max, but don't
             * move the others.
             */
            for (i = 0; i < maximum_colors; i++)
            {
               if ((int)png_ptr->dither_sort[i] >= maximum_colors)
               {
                  do
                     j--;
                  while ((int)png_ptr->dither_sort[j] >= maximum_colors);
                  palette[i] = palette[j];
               }
            }
         }
         else
         {
            int j = num_palette;

            /* Move all the used colors inside the max limit, and
             * develop a translation table.
             */
            for (i = 0; i < maximum_colors; i++)
            {
               /* Only move the colors we need to */
               if ((int)png_ptr->dither_sort[i] >= maximum_colors)
               {
                  png_color tmp_color;

                  do
                     j--;
                  while ((int)png_ptr->dither_sort[j] >= maximum_colors);

                  tmp_color = palette[j];
                  palette[j] = palette[i];
                  palette[i] = tmp_color;
                  /* Indicate where the color went */
                  png_ptr->dither_index[j] = (png_byte)i;
                  png_ptr->dither_index[i] = (png_byte)j;
               }
            }

            /* Find closest color for those colors we are not using */
            for (i = 0; i < num_palette; i++)
            {
               if ((int)png_ptr->dither_index[i] >= maximum_colors)
               {
                  int min_d, k, min_k, d_index;

                  /* Find the closest color to one we threw out */
                  d_index = png_ptr->dither_index[i];
                  min_d = PNG_COLOR_DIST(palette[d_index], palette[0]);
                  for (k = 1, min_k = 0; k < maximum_colors; k++)
                  {
                     int d;

                     d = PNG_COLOR_DIST(palette[d_index], palette[k]);

                     if (d < min_d)
                     {
                        min_d = d;
                        min_k = k;
                     }
                  }
                  /* Point to closest color */
                  png_ptr->dither_index[i] = (png_byte)min_k;
               }
            }
         }
         png_free(png_ptr, png_ptr->dither_sort);
         png_ptr->dither_sort = NULL;
      }
      else
      {
         /* This is much harder to do simply (and quickly).  Perhaps
          * we need to go through a median cut routine, but those
          * don't always behave themselves with only a few colors
          * as input.  So we will just find the closest two colors,
          * and throw out one of them (chosen somewhat randomly).
          * [We don't understand this at all, so if someone wants to
          *  work on improving it, be our guest - AED, GRP]
          */
         int i;
         int max_d;
         int num_new_palette;
         png_dsortp t;
         png_dsortpp hash;

         t = NULL;

         /* Initialize palette index arrays */
         png_ptr->index_to_palette = (png_bytep)png_malloc(png_ptr,
            (png_uint_32)(num_palette * png_sizeof(png_byte)));
         png_ptr->palette_to_index = (png_bytep)png_malloc(png_ptr,
            (png_uint_32)(num_palette * png_sizeof(png_byte)));

         /* Initialize the sort array */
         for (i = 0; i < num_palette; i++)
         {
            png_ptr->index_to_palette[i] = (png_byte)i;
            png_ptr->palette_to_index[i] = (png_byte)i;
         }

         hash = (png_dsortpp)png_calloc(png_ptr, (png_uint_32)(769 *
            png_sizeof(png_dsortp)));

         num_new_palette = num_palette;

         /* Initial wild guess at how far apart the farthest pixel
          * pair we will be eliminating will be.  Larger
          * numbers mean more areas will be allocated, Smaller
          * numbers run the risk of not saving enough data, and
          * having to do this all over again.
          *
          * I have not done extensive checking on this number.
          */
         max_d = 96;

         while (num_new_palette > maximum_colors)
         {
            for (i = 0; i < num_new_palette - 1; i++)
            {
               int j;

               for (j = i + 1; j < num_new_palette; j++)
               {
                  int d;

                  d = PNG_COLOR_DIST(palette[i], palette[j]);

                  if (d <= max_d)
                  {

                     t = (png_dsortp)png_malloc_warn(png_ptr,
                         (png_uint_32)(png_sizeof(png_dsort)));
                     if (t == NULL)
                         break;
                     t->next = hash[d];
                     t->left = (png_byte)i;
                     t->right = (png_byte)j;
                     hash[d] = t;
                  }
               }
               if (t == NULL)
                  break;
            }

            if (t != NULL)
            for (i = 0; i <= max_d; i++)
            {
               if (hash[i] != NULL)
               {
                  png_dsortp p;

                  for (p = hash[i]; p; p = p->next)
                  {
                     if ((int)png_ptr->index_to_palette[p->left]
                        < num_new_palette &&
                        (int)png_ptr->index_to_palette[p->right]
                        < num_new_palette)
                     {
                        int j, next_j;

                        if (num_new_palette & 0x01)
                        {
                           j = p->left;
                           next_j = p->right;
                        }
                        else
                        {
                           j = p->right;
                           next_j = p->left;
                        }

                        num_new_palette--;
                        palette[png_ptr->index_to_palette[j]]
                          = palette[num_new_palette];
                        if (!full_dither)
                        {
                           int k;

                           for (k = 0; k < num_palette; k++)
                           {
                              if (png_ptr->dither_index[k] ==
                                 png_ptr->index_to_palette[j])
                                 png_ptr->dither_index[k] =
                                    png_ptr->index_to_palette[next_j];
                              if ((int)png_ptr->dither_index[k] ==
                                 num_new_palette)
                                 png_ptr->dither_index[k] =
                                    png_ptr->index_to_palette[j];
                           }
                        }

                        png_ptr->index_to_palette[png_ptr->palette_to_index
                           [num_new_palette]] = png_ptr->index_to_palette[j];
                        png_ptr->palette_to_index[png_ptr->index_to_palette[j]]
                           = png_ptr->palette_to_index[num_new_palette];

                        png_ptr->index_to_palette[j] =
                            (png_byte)num_new_palette;
                        png_ptr->palette_to_index[num_new_palette] =
                            (png_byte)j;
                     }
                     if (num_new_palette <= maximum_colors)
                        break;
                  }
                  if (num_new_palette <= maximum_colors)
                     break;
               }
            }

            for (i = 0; i < 769; i++)
            {
               if (hash[i] != NULL)
               {
                  png_dsortp p = hash[i];
                  while (p)
                  {
                     t = p->next;
                     png_free(png_ptr, p);
                     p = t;
                  }
               }
               hash[i] = 0;
            }
            max_d += 96;
         }
         png_free(png_ptr, hash);
         png_free(png_ptr, png_ptr->palette_to_index);
         png_free(png_ptr, png_ptr->index_to_palette);
         png_ptr->palette_to_index = NULL;
         png_ptr->index_to_palette = NULL;
      }
      num_palette = maximum_colors;
   }
   if (png_ptr->palette == NULL)
   {
      png_ptr->palette = palette;
   }
   png_ptr->num_palette = (png_uint_16)num_palette;

   if (full_dither)
   {
      int i;
      png_bytep distance;
      int total_bits = PNG_DITHER_RED_BITS + PNG_DITHER_GREEN_BITS +
         PNG_DITHER_BLUE_BITS;
      int num_red = (1 << PNG_DITHER_RED_BITS);
      int num_green = (1 << PNG_DITHER_GREEN_BITS);
      int num_blue = (1 << PNG_DITHER_BLUE_BITS);
      png_size_t num_entries = ((png_size_t)1 << total_bits);

      png_ptr->palette_lookup = (png_bytep )png_calloc(png_ptr,
         (png_uint_32)(num_entries * png_sizeof(png_byte)));

      distance = (png_bytep)png_malloc(png_ptr, (png_uint_32)(num_entries *
         png_sizeof(png_byte)));
      png_memset(distance, 0xff, num_entries * png_sizeof(png_byte));

      for (i = 0; i < num_palette; i++)
      {
         int ir, ig, ib;
         int r = (palette[i].red >> (8 - PNG_DITHER_RED_BITS));
         int g = (palette[i].green >> (8 - PNG_DITHER_GREEN_BITS));
         int b = (palette[i].blue >> (8 - PNG_DITHER_BLUE_BITS));

         for (ir = 0; ir < num_red; ir++)
         {
            /* int dr = abs(ir - r); */
            int dr = ((ir > r) ? ir - r : r - ir);
            int index_r = (ir << (PNG_DITHER_BLUE_BITS +
                PNG_DITHER_GREEN_BITS));

            for (ig = 0; ig < num_green; ig++)
            {
               /* int dg = abs(ig - g); */
               int dg = ((ig > g) ? ig - g : g - ig);
               int dt = dr + dg;
               int dm = ((dr > dg) ? dr : dg);
               int index_g = index_r | (ig << PNG_DITHER_BLUE_BITS);

               for (ib = 0; ib < num_blue; ib++)
               {
                  int d_index = index_g | ib;
                  /* int db = abs(ib - b); */
                  int db = ((ib > b) ? ib - b : b - ib);
                  int dmax = ((dm > db) ? dm : db);
                  int d = dmax + dt + db;

                  if (d < (int)distance[d_index])
                  {
                     distance[d_index] = (png_byte)d;
                     png_ptr->palette_lookup[d_index] = (png_byte)i;
                  }
               }
            }
         }
      }

      png_free(png_ptr, distance);
   }
}
