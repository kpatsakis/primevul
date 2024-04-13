props_free (cherokee_validator_ldap_props_t *props)
{
	cherokee_buffer_mrproper (&props->server);
	cherokee_buffer_mrproper (&props->binddn);
	cherokee_buffer_mrproper (&props->bindpw);
	cherokee_buffer_mrproper (&props->basedn);
	cherokee_buffer_mrproper (&props->filter);
	cherokee_buffer_mrproper (&props->ca_file);

	return cherokee_validator_props_free_base (VALIDATOR_PROPS(props));
}
