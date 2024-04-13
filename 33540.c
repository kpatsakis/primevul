static void stroke_export(private_stroke_socket_t *this,
						  stroke_msg_t *msg, FILE *out)
{
	pop_string(msg, &msg->export.selector);

	if (msg->export.flags & EXPORT_X509)
	{
		enumerator_t *enumerator;
		identification_t *id;
		certificate_t *cert;

		id = identification_create_from_string(msg->export.selector);
		enumerator = lib->credmgr->create_cert_enumerator(lib->credmgr,
												CERT_X509, KEY_ANY, id, FALSE);
		while (enumerator->enumerate(enumerator, &cert))
		{
			print_pem_cert(out, cert);
		}
		enumerator->destroy(enumerator);
		id->destroy(id);
	}

	if (msg->export.flags & (EXPORT_CONN_CERT | EXPORT_CONN_CHAIN))
	{
		enumerator_t *sas, *auths, *certs;
		ike_sa_t *ike_sa;
		auth_cfg_t *auth;
		certificate_t *cert;
		auth_rule_t rule;

		sas = charon->ike_sa_manager->create_enumerator(
												charon->ike_sa_manager, TRUE);
		while (sas->enumerate(sas, &ike_sa))
		{
			if (streq(msg->export.selector, ike_sa->get_name(ike_sa)))
			{
				auths = ike_sa->create_auth_cfg_enumerator(ike_sa, FALSE);
				while (auths->enumerate(auths, &auth))
				{
					bool got_subject = FALSE;

					certs = auth->create_enumerator(auth);
					while (certs->enumerate(certs, &rule, &cert))
					{
						switch (rule)
						{
							case AUTH_RULE_CA_CERT:
							case AUTH_RULE_IM_CERT:
								if (msg->export.flags & EXPORT_CONN_CHAIN)
								{
									print_pem_cert(out, cert);
								}
								break;
							case AUTH_RULE_SUBJECT_CERT:
								if (!got_subject)
								{
									print_pem_cert(out, cert);
									got_subject = TRUE;
								}
								break;
							default:
								break;
						}
					}
					certs->destroy(certs);
				}
				auths->destroy(auths);
			}
		}
		sas->destroy(sas);
	}
}
