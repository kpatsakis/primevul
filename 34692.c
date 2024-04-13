save(ImlibImage * im, ImlibProgressFunction progress, char progress_granularity)
{
   int                 rc;
   FILE               *f;
   DATA8              *buf, *bptr;
   DATA32             *ptr;
   int                 x, y, pl = 0;
   char                pper = 0;

   /* no image data? abort */
   if (!im->data)
      return 0;
   f = fopen(im->real_file, "wb");
   if (!f)
      return 0;

   rc = 0;                      /* Error */

   /* if the image has a useful alpha channel */
   if (im->flags & F_HAS_ALPHA)
     {
        /* allocate a small buffer to convert image data */
        buf = malloc(im->w * 4 * sizeof(DATA8));
        if (!buf)
           goto quit;

        ptr = im->data;
        fprintf(f, "P8\n" "# PNM File written by Imlib2\n" "%i %i\n" "255\n",
                im->w, im->h);
        for (y = 0; y < im->h; y++)
          {
             bptr = buf;
             for (x = 0; x < im->w; x++)
               {
                  bptr[0] = ((*ptr) >> 16) & 0xff;
                  bptr[1] = ((*ptr) >> 8) & 0xff;
                  bptr[2] = ((*ptr)) & 0xff;
                  bptr[3] = ((*ptr) >> 24) & 0xff;
                  bptr += 4;
                  ptr++;
               }
             fwrite(buf, im->w * 4, 1, f);
             if (progress &&
                 do_progress(im, progress, progress_granularity, &pper, &pl, y))
                goto quit_progress;
          }
     }
   else
     {
        /* allocate a small buffer to convert image data */
        buf = malloc(im->w * 3 * sizeof(DATA8));
        if (!buf)
           goto quit;

        ptr = im->data;
        fprintf(f, "P6\n" "# PNM File written by Imlib2\n" "%i %i\n" "255\n",
                im->w, im->h);
        for (y = 0; y < im->h; y++)
          {
             bptr = buf;
             for (x = 0; x < im->w; x++)
               {
                  bptr[0] = ((*ptr) >> 16) & 0xff;
                  bptr[1] = ((*ptr) >> 8) & 0xff;
                  bptr[2] = ((*ptr)) & 0xff;
                  bptr += 3;
                  ptr++;
               }
             fwrite(buf, im->w * 3, 1, f);
             if (progress &&
                 do_progress(im, progress, progress_granularity, &pper, &pl, y))
                goto quit_progress;
          }
     }

   rc = 1;                      /* Ok */

   /* finish off */
   free(buf);
 quit:
   fclose(f);
   return rc;
 quit_progress:
   rc = 2;
   goto quit;
}
