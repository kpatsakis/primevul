_dbus_get_tmpdir(void)
{
  static const char* tmpdir = NULL;
  static char buf[1000];

  if (tmpdir == NULL)
    {
      char *last_slash;

      if (!GetTempPathA (sizeof (buf), buf))
        {
          _dbus_warn ("GetTempPath failed\n");
          _dbus_abort ();
        }

      /* Drop terminating backslash or slash */
      last_slash = _mbsrchr (buf, '\\');
      if (last_slash > buf && last_slash[1] == '\0')
        last_slash[0] = '\0';
      last_slash = _mbsrchr (buf, '/');
      if (last_slash > buf && last_slash[1] == '\0')
        last_slash[0] = '\0';

      tmpdir = buf;
    }

  _dbus_assert(tmpdir != NULL);

  return tmpdir;
}
