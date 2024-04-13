prepend_default_mount_options (const FSMountOptions *fsmo,
                               uid_t caller_uid,
                               char **given_options)
{
  GPtrArray *options;
  int n;
  char *s;
  gid_t gid;

  options = g_ptr_array_new ();
  if (fsmo != NULL)
    {
      for (n = 0; fsmo->defaults != NULL && fsmo->defaults[n] != NULL; n++)
        {
          const char *option = fsmo->defaults[n];

          if (strcmp (option, "uid=") == 0)
            {
              s = g_strdup_printf ("uid=%d", caller_uid);
              g_ptr_array_add (options, s);
            }
          else if (strcmp (option, "gid=") == 0)
            {
              gid = find_primary_gid (caller_uid);
              if (gid != (gid_t) - 1)
                {
                  s = g_strdup_printf ("gid=%d", gid);
                  g_ptr_array_add (options, s);
                }
            }
          else
            {
              g_ptr_array_add (options, g_strdup (option));
            }
        }
    }
  for (n = 0; given_options[n] != NULL; n++)
    {
      g_ptr_array_add (options, g_strdup (given_options[n]));
    }

  g_ptr_array_add (options, NULL);

  return (char **) g_ptr_array_free (options, FALSE);
}
