cherokee_validator_ldap_configure (cherokee_config_node_t *conf, cherokee_server_t *srv, cherokee_module_props_t **_props)
{
	ret_t                            ret;
	cherokee_list_t                 *i;
	cherokee_validator_ldap_props_t *props;

	UNUSED(srv);

	if (*_props == NULL) {
		CHEROKEE_NEW_STRUCT (n, validator_ldap_props);

		cherokee_validator_props_init_base (VALIDATOR_PROPS(n), MODULE_PROPS_FREE(props_free));

		n->port = LDAP_DEFAULT_PORT;
		n->tls  = false;

		cherokee_buffer_init (&n->server);
		cherokee_buffer_init (&n->binddn);
		cherokee_buffer_init (&n->bindpw);
		cherokee_buffer_init (&n->basedn);
		cherokee_buffer_init (&n->filter);
		cherokee_buffer_init (&n->ca_file);

		*_props = MODULE_PROPS(n);
	}

	props = PROP_LDAP(*_props);

	cherokee_config_node_foreach (i, conf) {
		cherokee_config_node_t *subconf = CONFIG_NODE(i);

		if (equal_buf_str (&subconf->key, "server")) {
			cherokee_buffer_add_buffer (&props->server, &subconf->val);

		} else if (equal_buf_str (&subconf->key, "port")) {
			ret = cherokee_atoi (subconf->val.buf, &props->port);
			if (ret != ret_ok) return ret_error;

		} else if (equal_buf_str (&subconf->key, "bind_dn")) {
			cherokee_buffer_add_buffer (&props->binddn, &subconf->val);

		} else if (equal_buf_str (&subconf->key, "bind_pw")) {
			cherokee_buffer_add_buffer (&props->bindpw, &subconf->val);

		} else if (equal_buf_str (&subconf->key, "base_dn")) {
			cherokee_buffer_add_buffer (&props->basedn, &subconf->val);

		} else if (equal_buf_str (&subconf->key, "filter")) {
			cherokee_buffer_add_buffer (&props->filter, &subconf->val);

		} else if (equal_buf_str (&subconf->key, "tls")) {
			ret = cherokee_atob (subconf->val.buf, &props->tls);
			if (ret != ret_ok) return ret_error;

		} else if (equal_buf_str (&subconf->key, "ca_file")) {
			cherokee_buffer_add_buffer (&props->ca_file, &subconf->val);

		} else if (equal_buf_str (&subconf->key, "methods") ||
		           equal_buf_str (&subconf->key, "realm")   ||
		           equal_buf_str (&subconf->key, "users")) {
			/* Handled in validator.c
			 */
		} else {
			LOG_WARNING (CHEROKEE_ERROR_VALIDATOR_LDAP_KEY, subconf->key.buf);
		}
	}

	/* Checks
	 */
	if (cherokee_buffer_is_empty (&props->basedn)) {
		LOG_ERROR (CHEROKEE_ERROR_VALIDATOR_LDAP_PROPERTY, "base_dn");
		return ret_error;
	}

	if (cherokee_buffer_is_empty (&props->server)) {
		LOG_ERROR (CHEROKEE_ERROR_VALIDATOR_LDAP_PROPERTY, "server");
		return ret_error;
	}

	if ((cherokee_buffer_is_empty (&props->bindpw) &&
	     (! cherokee_buffer_is_empty (&props->basedn))))
	{
		LOG_ERROR_S (CHEROKEE_ERROR_VALIDATOR_LDAP_SECURITY);
		return ret_error;
	}

	return ret_ok;
}
