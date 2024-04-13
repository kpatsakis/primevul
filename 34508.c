may_identity_check_authorization (PolkitBackendInteractiveAuthority   *interactive_authority,
                                  const gchar                         *action_id,
                                  PolkitIdentity                      *identity)
{
  PolkitBackendInteractiveAuthorityPrivate *priv = POLKIT_BACKEND_INTERACTIVE_AUTHORITY_GET_PRIVATE (interactive_authority);
  gboolean ret = FALSE;
  PolkitActionDescription *action_desc = NULL;
  const gchar *owners = NULL;
  gchar **tokens = NULL;
  guint n;

  /* uid 0 may check anything */
  if (identity_is_root_user (identity))
    {
      ret = TRUE;
      goto out;
    }

  action_desc = polkit_backend_action_pool_get_action (priv->action_pool, action_id, NULL);
  if (action_desc == NULL)
    goto out;

  owners = polkit_action_description_get_annotation (action_desc, "org.freedesktop.policykit.owner");
  if (owners == NULL)
    goto out;

  tokens = g_strsplit (owners, " ", 0);
  for (n = 0; tokens != NULL && tokens[n] != NULL; n++)
    {
      PolkitIdentity *owner_identity;
      GError *error = NULL;
      owner_identity = polkit_identity_from_string (tokens[n], &error);
      if (owner_identity == NULL)
        {
          g_warning ("Error parsing owner identity %d of action_id %s: %s (%s, %d)",
                     n, action_id, error->message, g_quark_to_string (error->domain), error->code);
          g_error_free (error);
          continue;
        }
      if (polkit_identity_equal (identity, owner_identity))
        {
          ret = TRUE;
          g_object_unref (owner_identity);
          goto out;
        }
      g_object_unref (owner_identity);
    }

 out:
  g_clear_object (&action_desc);
  g_strfreev (tokens);

  return ret;
}
