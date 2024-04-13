static unsigned char *ikev2_authloc(struct msg_digest *md,
				    pb_stream *e_pbs)
{
	unsigned char *b12;
	struct state *st = md->st;
	struct state *pst = st;

	if (st->st_clonedfrom != 0) {
		pst = state_with_serialno(st->st_clonedfrom);
		if ( pst == NULL)
			return NULL;
	}

	b12 = e_pbs->cur;
	if (!out_zero(pst->st_oakley.integ_hasher->hash_integ_len, e_pbs,
		      "length of truncated HMAC"))
		return NULL;

	return b12;
}
