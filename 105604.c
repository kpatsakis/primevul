sysfs_get_string (const char *dir,
                  const char *attribute)
{
  char *result;
  char *filename;

  result = NULL;
  filename = g_build_filename (dir, attribute, NULL);
  if (!g_file_get_contents (filename, &result, NULL, NULL))
    {
      result = g_strdup ("");
    }
  g_free (filename);

  return result;
}
