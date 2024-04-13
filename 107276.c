inf_gtk_certificate_manager_class_init(
  InfGtkCertificateManagerClass* certificate_manager_class)
{
  GObjectClass* object_class;
  object_class = G_OBJECT_CLASS(certificate_manager_class);

  object_class->dispose = inf_gtk_certificate_manager_dispose;
  object_class->finalize = inf_gtk_certificate_manager_finalize;
  object_class->set_property = inf_gtk_certificate_manager_set_property;
  object_class->get_property = inf_gtk_certificate_manager_get_property;

  g_object_class_install_property(
    object_class,
    PROP_PARENT_WINDOW,
    g_param_spec_object(
      "parent-window",
      "Parent window",
      "The parent window for certificate approval dialogs",
      GTK_TYPE_WINDOW,
      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY
    )
  );

  g_object_class_install_property(
    object_class,
    PROP_XMPP_MANAGER,
    g_param_spec_object(
      "xmpp-manager",
      "XMPP manager",
      "The XMPP manager of registered connections",
      INF_TYPE_XMPP_MANAGER,
      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY
    )
  );

  g_object_class_install_property(
    object_class,
    PROP_KNOWN_HOSTS_FILE,
    g_param_spec_string(
      "known-hosts-file",
      "Known hosts file",
      "File containing certificates of known hosts",
      NULL,
      G_PARAM_READWRITE
    )
  );
}
