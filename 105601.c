sysfs_file_exists (const char *dir,
                   const char *attribute)
{
  gboolean result;
  char *filename;

  result = FALSE;
  filename = g_build_filename (dir, attribute, NULL);
  if (g_file_test (filename, G_FILE_TEST_EXISTS))
    {
      result = TRUE;
    }
  g_free (filename);

  return result;
}
