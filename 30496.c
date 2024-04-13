_dbus_connect_exec (const char     *path,
                    char *const    argv[],
                    DBusError      *error)
{
  int fds[2];
  pid_t pid;

  _DBUS_ASSERT_ERROR_IS_CLEAR (error);

  _dbus_verbose ("connecting to process %s\n", path);

  if (socketpair (AF_UNIX, SOCK_STREAM
#ifdef SOCK_CLOEXEC
                  |SOCK_CLOEXEC
#endif
                  , 0, fds) < 0)
    {
      dbus_set_error (error,
                      _dbus_error_from_errno (errno),
                      "Failed to create socket pair: %s",
                      _dbus_strerror (errno));
      return -1;
    }

  _dbus_fd_set_close_on_exec (fds[0]);
  _dbus_fd_set_close_on_exec (fds[1]);

  pid = fork ();
  if (pid < 0)
    {
      dbus_set_error (error,
                      _dbus_error_from_errno (errno),
                      "Failed to fork() to call %s: %s",
                      path, _dbus_strerror (errno));
      close (fds[0]);
      close (fds[1]);
      return -1;
    }

  if (pid == 0)
    {
      /* child */
      close (fds[0]);

      dup2 (fds[1], STDIN_FILENO);
      dup2 (fds[1], STDOUT_FILENO);

      if (fds[1] != STDIN_FILENO &&
          fds[1] != STDOUT_FILENO)
        close (fds[1]);

      /* Inherit STDERR and the controlling terminal from the
         parent */

      _dbus_close_all ();

      execvp (path, argv);

      fprintf (stderr, "Failed to execute process %s: %s\n", path, _dbus_strerror (errno));

      _exit(1);
    }

  /* parent */
  close (fds[1]);

  if (!_dbus_set_fd_nonblocking (fds[0], error))
    {
      _DBUS_ASSERT_ERROR_IS_SET (error);

      close (fds[0]);
      return -1;
    }

  return fds[0];
}
