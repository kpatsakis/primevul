sys_log (GDBusMethodInvocation *context,
         const gchar           *format,
                                ...)
{
        va_list args;
        gchar *msg;

        va_start (args, format);
        msg = g_strdup_vprintf (format, args);
        va_end (args);

        if (context) {
                PolkitSubject *subject;
                gchar *cmdline;
                gchar *id;
                gint pid = 0;
                gint uid = 0;
                gchar *tmp;

                subject = polkit_system_bus_name_new (g_dbus_method_invocation_get_sender (context));
                id = polkit_subject_to_string (subject);
                cmdline = _polkit_subject_get_cmdline (subject, &pid, &uid);

                if (cmdline == NULL) {
                        tmp = g_strdup_printf ("request by %s: %s", id, msg);
                }
                else {
                        tmp = g_strdup_printf ("request by %s [%s pid:%d uid:%d]: %s", id, cmdline, pid, uid, msg);
                }

                g_free (msg);
                msg = tmp;

                g_free (id);
                g_free (cmdline);
                g_object_unref (subject);
        }

        syslog (LOG_NOTICE, "%s", msg);

        g_free (msg);
}
