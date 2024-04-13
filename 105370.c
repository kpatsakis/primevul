compute_object_path (const char *native_path)
{
  const gchar *basename;
  GString *s;
  guint n;

  g_return_val_if_fail (native_path != NULL, NULL);

  basename = strrchr (native_path, '/');
  if (basename != NULL)
    {
      basename++;
    }
  else
    {
      basename = native_path;
    }

  s = g_string_new ("/org/freedesktop/UDisks/devices/");
  for (n = 0; basename[n] != '\0'; n++)
    {
      gint c = basename[n];

      /* D-Bus spec sez:
       *
       * Each element must only contain the ASCII characters "[A-Z][a-z][0-9]_"
       */
      if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
        {
          g_string_append_c (s, c);
        }
      else
        {
          /* Escape bytes not in [A-Z][a-z][0-9] as _<hex-with-two-digits> */
          g_string_append_printf (s, "_%02x", c);
        }
    }

  return g_string_free (s, FALSE);
}
