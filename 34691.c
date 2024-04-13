formats(ImlibLoader * l)
{
   static const char  *const list_formats[] =
      { "pnm", "ppm", "pgm", "pbm", "pam" };
   int                 i;

   l->num_formats = sizeof(list_formats) / sizeof(char *);
   l->formats = malloc(sizeof(char *) * l->num_formats);

   for (i = 0; i < l->num_formats; i++)
      l->formats[i] = strdup(list_formats[i]);
}
