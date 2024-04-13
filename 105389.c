daemon_linux_md_create (Daemon *daemon,
                        GPtrArray *components,
                        char *level,
                        guint64 stripe_size,
                        char *name,
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
                           "LinuxMdCreate",
                           TRUE,
                           daemon_linux_md_create_authorized_cb,
                           context,
                           4,
                           components_as_strv,
                           g_strfreev,
                           g_strdup (level),
                           g_free,
                           g_memdup (&stripe_size, sizeof(guint64)),
                           g_free,
                           g_strdup (name),
                           g_free,
                           g_strdupv (options),
                           g_strfreev);

  return TRUE;
}
