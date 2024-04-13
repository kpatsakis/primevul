polkit_backend_session_monitor_finalize (GObject *object)
{
  PolkitBackendSessionMonitor *monitor = POLKIT_BACKEND_SESSION_MONITOR (object);

  if (monitor->system_bus != NULL)
    g_object_unref (monitor->system_bus);

  if (monitor->sd_source != NULL)
    {
      g_source_destroy (monitor->sd_source);
      g_source_unref (monitor->sd_source);
    }

  if (G_OBJECT_CLASS (polkit_backend_session_monitor_parent_class)->finalize != NULL)
    G_OBJECT_CLASS (polkit_backend_session_monitor_parent_class)->finalize (object);
}
