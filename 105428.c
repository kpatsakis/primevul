device_generate_kernel_change_event (Device *device)
{
  FILE *f;
  char *filename;

  filename = g_build_filename (device->priv->native_path, "uevent", NULL);
  f = fopen (filename, "w");
  if (f == NULL)
    {
      g_warning ("error opening %s for writing: %m", filename);
    }
  else
    {
      if (fputs ("change", f) == EOF)
        {
          g_warning ("error writing 'change' to %s: %m", filename);
        }
      fclose (f);
    }
  g_free (filename);
}
