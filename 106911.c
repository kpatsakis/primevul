static stf_status ikev2_send_auth(struct connection *c,
				  struct state *st,
				  enum phase1_role role,
				  unsigned int np,
				  unsigned char *idhash_out,
				  pb_stream *outpbs)
{
	struct ikev2_a a;
	pb_stream a_pbs;
	struct state *pst = st;

	if (st->st_clonedfrom != 0)
		pst = state_with_serialno(st->st_clonedfrom);

	a.isaa_critical = ISAKMP_PAYLOAD_NONCRITICAL;
	if (DBGP(IMPAIR_SEND_BOGUS_ISAKMP_FLAG)) {
		libreswan_log(
			" setting bogus ISAKMP_PAYLOAD_LIBRESWAN_BOGUS flag in ISAKMP payload");
		a.isaa_critical |= ISAKMP_PAYLOAD_LIBRESWAN_BOGUS;
	}

	a.isaa_np = np;

	if (c->policy & POLICY_RSASIG) {
		a.isaa_type = v2_AUTH_RSA;
	} else if (c->policy & POLICY_PSK) {
		a.isaa_type = v2_AUTH_SHARED;
	} else {
		/* what else is there?... DSS not implemented. */
		return STF_FAIL;
	}

	if (!out_struct(&a,
			&ikev2_a_desc,
			outpbs,
			&a_pbs))
		return STF_INTERNAL_ERROR;

	if (c->policy & POLICY_RSASIG) {
		if (!ikev2_calculate_rsa_sha1(pst, role, idhash_out, &a_pbs))
			return STF_FATAL + v2N_AUTHENTICATION_FAILED;

	} else if (c->policy & POLICY_PSK) {
		if (!ikev2_calculate_psk_auth(pst, role, idhash_out, &a_pbs))
			return STF_FAIL + v2N_AUTHENTICATION_FAILED;
	}

	close_output_pbs(&a_pbs);
	return STF_OK;
}
