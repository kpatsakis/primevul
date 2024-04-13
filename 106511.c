cherokee_validator_ldap_new (cherokee_validator_ldap_t **ldap, cherokee_module_props_t *props)
{
	ret_t ret;
	CHEROKEE_NEW_STRUCT(n,validator_ldap);

	/* Init
	 */
	cherokee_validator_init_base (VALIDATOR(n), VALIDATOR_PROPS(props), PLUGIN_INFO_VALIDATOR_PTR(ldap));
	VALIDATOR(n)->support = http_auth_basic;

	MODULE(n)->free           = (module_func_free_t)           cherokee_validator_ldap_free;
	VALIDATOR(n)->check       = (validator_func_check_t)       cherokee_validator_ldap_check;
	VALIDATOR(n)->add_headers = (validator_func_add_headers_t) cherokee_validator_ldap_add_headers;

	/* Init properties
	 */
	cherokee_buffer_init (&n->filter);

	/* Connect to the LDAP server
	 */
	ret = init_ldap_connection (n, PROP_LDAP(props));
	if (ret != ret_ok) {
		cherokee_validator_free (VALIDATOR(n));
		return ret;
	}

	*ldap = n;
	return ret_ok;
}
