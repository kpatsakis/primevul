daemon_linux_md_start (Daemon *daemon,
                       GPtrArray *components,
                       char **options,
                       DBusGMethodInvocation *context)
{
  gchar **components_as_strv;
  guint n;

  components_as_strv = g_new0 (gchar *, components->len + 1);
  for (n = 0; n < components->len; n++)
    components_as_strv[n] = g_strdup (components->pdata[n]);

  daemon_local_check_auth (daemon,
                           NULL,
                           "org.freedesktop.udisks.linux-md",
                           "LinuxMdStart",
                           TRUE,
                           daemon_linux_md_start_authorized_cb,
                           context,
                           2,
                           components_as_strv,
                           g_strfreev,
                           g_strdupv (options),
                           g_strfreev);

  return TRUE;
}
