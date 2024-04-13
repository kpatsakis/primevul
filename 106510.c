cherokee_validator_ldap_free (cherokee_validator_ldap_t *ldap)
{
	cherokee_buffer_mrproper (&ldap->filter);

	if (ldap->conn)
		ldap_unbind (ldap->conn);

	return ret_ok;
}
