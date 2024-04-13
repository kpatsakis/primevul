ensure_database (PolkitBackendSessionMonitor  *monitor,
                 GError                      **error)
{
  gboolean ret = FALSE;

  if (monitor->database != NULL)
    {
      struct stat statbuf;

      if (stat (CKDB_PATH, &statbuf) != 0)
        {
          g_set_error (error,
                       G_IO_ERROR,
                       g_io_error_from_errno (errno),
                       "Error statting file " CKDB_PATH " to check timestamp: %s",
                       strerror (errno));
          goto out;
        }
      if (statbuf.st_mtime == monitor->database_mtime)
        {
          ret = TRUE;
          goto out;
        }
    }

  ret = reload_database (monitor, error);

 out:
  return ret;
}
