get_caller_loginuid (GDBusMethodInvocation *context, gchar *loginuid, gint size)
{
        PolkitSubject *subject;
        gchar *cmdline;
        gint pid;
        gint uid;
        gchar *path;
        gchar *buf;

        subject = polkit_system_bus_name_new (g_dbus_method_invocation_get_sender (context));
        cmdline = _polkit_subject_get_cmdline (subject, &pid, &uid);
        g_free (cmdline);
        g_object_unref (subject);

        path = g_strdup_printf ("/proc/%d/loginuid", pid);
        if (g_file_get_contents (path, &buf, NULL, NULL)) {
                strncpy (loginuid, buf, size);
                g_free (buf);
        }
        else {
                g_snprintf (loginuid, size, "%d", uid);
        }

        g_free (path);
}
