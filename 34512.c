polkit_backend_interactive_authority_class_init (PolkitBackendInteractiveAuthorityClass *klass)
{
  GObjectClass *gobject_class;
  PolkitBackendAuthorityClass *authority_class;

  gobject_class = G_OBJECT_CLASS (klass);
  authority_class = POLKIT_BACKEND_AUTHORITY_CLASS (klass);

  gobject_class->finalize = polkit_backend_interactive_authority_finalize;

  authority_class->get_name                        = polkit_backend_interactive_authority_get_name;
  authority_class->get_version                     = polkit_backend_interactive_authority_get_version;
  authority_class->get_features                    = polkit_backend_interactive_authority_get_features;
  authority_class->enumerate_actions               = polkit_backend_interactive_authority_enumerate_actions;
  authority_class->check_authorization             = polkit_backend_interactive_authority_check_authorization;
  authority_class->check_authorization_finish      = polkit_backend_interactive_authority_check_authorization_finish;
  authority_class->register_authentication_agent   = polkit_backend_interactive_authority_register_authentication_agent;
  authority_class->unregister_authentication_agent = polkit_backend_interactive_authority_unregister_authentication_agent;
  authority_class->authentication_agent_response   = polkit_backend_interactive_authority_authentication_agent_response;
  authority_class->enumerate_temporary_authorizations = polkit_backend_interactive_authority_enumerate_temporary_authorizations;
  authority_class->revoke_temporary_authorizations = polkit_backend_interactive_authority_revoke_temporary_authorizations;
  authority_class->revoke_temporary_authorization_by_id = polkit_backend_interactive_authority_revoke_temporary_authorization_by_id;



  g_type_class_add_private (klass, sizeof (PolkitBackendInteractiveAuthorityPrivate));
}
