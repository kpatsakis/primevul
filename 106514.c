validate_dn (cherokee_validator_ldap_props_t *props, char *dn, char *password)
{
	int   re;
	int   val;
	LDAP *conn;

	conn = ldap_init (props->server.buf, props->port);
	if (conn == NULL) return ret_error;

	val = LDAP_VERSION3;
	re = ldap_set_option (conn, LDAP_OPT_PROTOCOL_VERSION, &val);
	if (re != LDAP_OPT_SUCCESS)
		goto error;

	if (props->tls) {
#ifdef LDAP_HAVE_START_TLS_S
		re = ldap_start_tls_s (conn, NULL,  NULL);
		if (re != LDAP_OPT_SUCCESS) {
			TRACE (ENTRIES, "Couldn't StartTLS\n");
			goto error;
		}
#else
		LOG_ERROR_S (CHEROKEE_ERROR_VALIDATOR_LDAP_STARTTLS);
#endif
	}

	re = ldap_simple_bind_s (conn, dn, password);
	if (re != LDAP_SUCCESS)
		goto error;

	return ret_ok;

error:
	ldap_unbind_s (conn);
	return ret_error;
}
