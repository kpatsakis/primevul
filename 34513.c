polkit_backend_interactive_authority_enumerate_actions (PolkitBackendAuthority   *authority,
                                                  PolkitSubject            *caller,
                                                  const gchar              *interactivee,
                                                  GError                  **error)
{
  PolkitBackendInteractiveAuthority *interactive_authority;
  PolkitBackendInteractiveAuthorityPrivate *priv;
  GList *actions;

  interactive_authority = POLKIT_BACKEND_INTERACTIVE_AUTHORITY (authority);
  priv = POLKIT_BACKEND_INTERACTIVE_AUTHORITY_GET_PRIVATE (interactive_authority);

  actions = polkit_backend_action_pool_get_all_actions (priv->action_pool, interactivee);

  return actions;
}
