smp_fetch_proto_http(const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	/* Note: hdr_idx.v cannot be NULL in this ACL because the ACL is tagged
	 * as a layer7 ACL, which involves automatic allocation of hdr_idx.
	 */

	CHECK_HTTP_MESSAGE_FIRST_PERM();

	smp->data.type = SMP_T_BOOL;
	smp->data.u.sint = 1;
	return 1;
}
