sysfs_get_double (const char *dir,
                  const char *attribute)
{
  double result;
  char *contents;
  char *filename;

  result = 0.0;
  filename = g_build_filename (dir, attribute, NULL);
  if (g_file_get_contents (filename, &contents, NULL, NULL))
    {
      result = atof (contents);
      g_free (contents);
    }
  g_free (filename);

  return result;
}
