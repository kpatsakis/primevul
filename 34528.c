sessions_changed (gpointer user_data)
{
  PolkitBackendSessionMonitor *monitor = POLKIT_BACKEND_SESSION_MONITOR (user_data);

  g_signal_emit (monitor, signals[CHANGED_SIGNAL], 0);

  return TRUE;
}
