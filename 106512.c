init_ldap_connection (cherokee_validator_ldap_t *ldap, cherokee_validator_ldap_props_t *props)
{
	int re;
	int val;

	/* Connect
	 */
	ldap->conn = ldap_init (props->server.buf, props->port);
	if (ldap->conn == NULL) {
		LOG_ERRNO (errno, cherokee_err_critical,
		           CHEROKEE_ERROR_VALIDATOR_LDAP_CONNECT,
		           props->server.buf, props->port);
		return ret_error;
	}

	TRACE (ENTRIES, "Connected to %s:%d\n", props->server.buf, props->port);

	/* Set LDAP protocol version
	 */
	val = LDAP_VERSION3;
	re = ldap_set_option (ldap->conn, LDAP_OPT_PROTOCOL_VERSION, &val);
	if (re != LDAP_OPT_SUCCESS) {
		LOG_ERROR (CHEROKEE_ERROR_VALIDATOR_LDAP_V3, ldap_err2string(re));
		return ret_error;
	}

	TRACE (ENTRIES, "LDAP protocol version %d set\n", LDAP_VERSION3);

	/* Secure connections
	 */
	if (props->tls) {
#ifdef LDAP_OPT_X_TLS
		if (! cherokee_buffer_is_empty (&props->ca_file)) {
			re = ldap_set_option (NULL, LDAP_OPT_X_TLS_CACERTFILE, props->ca_file.buf);
			if (re != LDAP_OPT_SUCCESS) {
				LOG_CRITICAL (CHEROKEE_ERROR_VALIDATOR_LDAP_CA,
				              props->ca_file.buf, ldap_err2string (re));
				return ret_error;
			}
		}
#else
		LOG_ERROR_S (CHEROKEE_ERROR_VALIDATOR_LDAP_STARTTLS);
#endif
	}

	/* Bind
	 */
	if (cherokee_buffer_is_empty (&props->binddn)) {
		TRACE (ENTRIES, "anonymous bind %s", "\n");
		re = ldap_simple_bind_s (ldap->conn, NULL, NULL);
	} else {
		TRACE (ENTRIES, "bind user=%s password=%s\n",
		       props->binddn.buf, props->bindpw.buf);
		re = ldap_simple_bind_s (ldap->conn, props->binddn.buf, props->bindpw.buf);
	}

	if (re != LDAP_SUCCESS) {
		LOG_CRITICAL (CHEROKEE_ERROR_VALIDATOR_LDAP_BIND,
		              props->server.buf, props->port, props->binddn.buf,
		              props->bindpw.buf, ldap_err2string(re));
		return ret_error;
	}

	return ret_ok;
}
