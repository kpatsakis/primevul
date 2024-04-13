check_authorization_challenge_cb (AuthenticationAgent         *agent,
                                  PolkitSubject               *subject,
                                  PolkitIdentity              *user_of_subject,
                                  PolkitSubject               *caller,
                                  PolkitBackendInteractiveAuthority *authority,
                                  const gchar                 *action_id,
                                  PolkitDetails               *details,
                                  PolkitImplicitAuthorization  implicit_authorization,
                                  gboolean                     authentication_success,
                                  gboolean                     was_dismissed,
                                  PolkitIdentity              *authenticated_identity,
                                  gpointer                     user_data)
{
  GSimpleAsyncResult *simple = G_SIMPLE_ASYNC_RESULT (user_data);
  PolkitBackendInteractiveAuthorityPrivate *priv;
  PolkitAuthorizationResult *result;
  gchar *scope_str;
  gchar *subject_str;
  gchar *user_of_subject_str;
  gchar *authenticated_identity_str;
  gchar *subject_cmdline;
  gboolean is_temp;

  priv = POLKIT_BACKEND_INTERACTIVE_AUTHORITY_GET_PRIVATE (authority);

  result = NULL;

  scope_str = polkit_subject_to_string (agent->scope);
  subject_str = polkit_subject_to_string (subject);
  user_of_subject_str = polkit_identity_to_string (user_of_subject);
  authenticated_identity_str = NULL;
  if (authenticated_identity != NULL)
    authenticated_identity_str = polkit_identity_to_string (authenticated_identity);

  subject_cmdline = _polkit_subject_get_cmdline (subject);
  if (subject_cmdline == NULL)
    subject_cmdline = g_strdup ("<unknown>");

  g_debug ("In check_authorization_challenge_cb\n"
           "  subject                %s\n"
           "  action_id              %s\n"
           "  was_dismissed          %d\n"
           "  authentication_success %d\n",
           subject_str,
           action_id,
           was_dismissed,
           authentication_success);

  if (implicit_authorization == POLKIT_IMPLICIT_AUTHORIZATION_AUTHENTICATION_REQUIRED_RETAINED ||
      implicit_authorization == POLKIT_IMPLICIT_AUTHORIZATION_ADMINISTRATOR_AUTHENTICATION_REQUIRED_RETAINED)
    polkit_details_insert (details, "polkit.retains_authorization_after_challenge", "true");

  is_temp = FALSE;
  if (authentication_success)
    {
      /* store temporary authorization depending on value of implicit_authorization */
      if (implicit_authorization == POLKIT_IMPLICIT_AUTHORIZATION_AUTHENTICATION_REQUIRED_RETAINED ||
          implicit_authorization == POLKIT_IMPLICIT_AUTHORIZATION_ADMINISTRATOR_AUTHENTICATION_REQUIRED_RETAINED)
        {
          const gchar *id;

          is_temp = TRUE;

          id = temporary_authorization_store_add_authorization (priv->temporary_authorization_store,
                                                                subject,
                                                                authentication_agent_get_scope (agent),
                                                                action_id);

          polkit_details_insert (details, "polkit.temporary_authorization_id", id);

          /* we've added a temporary authorization, let the user know */
          g_signal_emit_by_name (authority, "changed");
        }
      result = polkit_authorization_result_new (TRUE, FALSE, details);
    }
  else
    {
      /* TODO: maybe return set is_challenge? */
      if (was_dismissed)
        polkit_details_insert (details, "polkit.dismissed", "true");
      result = polkit_authorization_result_new (FALSE, FALSE, details);
    }

  /* Log the event */
  if (authentication_success)
    {
      if (is_temp)
        {
          polkit_backend_authority_log (POLKIT_BACKEND_AUTHORITY (authority),
                                        "Operator of %s successfully authenticated as %s to gain "
                                        "TEMPORARY authorization for action %s for %s [%s] (owned by %s)",
                                        scope_str,
                                        authenticated_identity_str,
                                        action_id,
                                        subject_str,
                                        subject_cmdline,
                                        user_of_subject_str);
        }
      else
        {
          polkit_backend_authority_log (POLKIT_BACKEND_AUTHORITY (authority),
                                        "Operator of %s successfully authenticated as %s to gain "
                                        "ONE-SHOT authorization for action %s for %s [%s] (owned by %s)",
                                        scope_str,
                                        authenticated_identity_str,
                                        action_id,
                                        subject_str,
                                        subject_cmdline,
                                        user_of_subject_str);
        }
    }
  else
    {
      polkit_backend_authority_log (POLKIT_BACKEND_AUTHORITY (authority),
                                    "Operator of %s FAILED to authenticate to gain "
                                    "authorization for action %s for %s [%s] (owned by %s)",
                                    scope_str,
                                    action_id,
                                    subject_str,
                                    subject_cmdline,
                                    user_of_subject_str);
    }

  /* log_result (authority, action_id, subject, caller, result); */

  g_simple_async_result_set_op_res_gpointer (simple,
                                             result,
                                             g_object_unref);
  g_simple_async_result_complete (simple);
  g_object_unref (simple);

  g_free (subject_cmdline);
  g_free (authenticated_identity_str);
  g_free (user_of_subject_str);
  g_free (subject_str);
  g_free (scope_str);
}
