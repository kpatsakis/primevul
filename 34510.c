on_session_monitor_changed (PolkitBackendSessionMonitor *monitor,
                            gpointer                     user_data)
{
  PolkitBackendInteractiveAuthority *authority = POLKIT_BACKEND_INTERACTIVE_AUTHORITY (user_data);
  g_signal_emit_by_name (authority, "changed");
}
