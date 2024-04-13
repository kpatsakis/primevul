_dbus_close_all (void)
{
  int maxfds, i;

#ifdef __linux__
  DIR *d;

  /* On Linux we can optimize this a bit if /proc is available. If it
     isn't available, fall back to the brute force way. */

  d = opendir ("/proc/self/fd");
  if (d)
    {
      for (;;)
        {
          struct dirent buf, *de;
          int k, fd;
          long l;
          char *e = NULL;

          k = readdir_r (d, &buf, &de);
          if (k != 0 || !de)
            break;

          if (de->d_name[0] == '.')
            continue;

          errno = 0;
          l = strtol (de->d_name, &e, 10);
          if (errno != 0 || e == NULL || *e != '\0')
            continue;

          fd = (int) l;
          if (fd < 3)
            continue;

          if (fd == dirfd (d))
            continue;

          close (fd);
        }

      closedir (d);
      return;
    }
#endif

  maxfds = sysconf (_SC_OPEN_MAX);

  /* Pick something reasonable if for some reason sysconf says
   * unlimited.
   */
  if (maxfds < 0)
    maxfds = 1024;

  /* close all inherited fds */
  for (i = 3; i < maxfds; i++)
    close (i);
}
