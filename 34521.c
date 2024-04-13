polkit_backend_session_monitor_init (PolkitBackendSessionMonitor *monitor)
{
  GError *error;

  error = NULL;
  monitor->system_bus = g_bus_get_sync (G_BUS_TYPE_SYSTEM, NULL, &error);
  if (monitor->system_bus == NULL)
    {
      g_printerr ("Error getting system bus: %s", error->message);
      g_error_free (error);
    }

  monitor->sd_source = sd_source_new ();
  g_source_set_callback (monitor->sd_source, sessions_changed, monitor, NULL);
  g_source_attach (monitor->sd_source, NULL);
}
