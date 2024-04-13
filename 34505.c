action_pool_changed (PolkitBackendActionPool *action_pool,
                     PolkitBackendInteractiveAuthority *authority)
{
  g_signal_emit_by_name (authority, "changed");
}
