 static void ikev2_padup_pre_encrypt(struct msg_digest *md,
				    pb_stream *e_pbs_cipher)
{
	struct state *st = md->st;
	struct state *pst = st;

	if (st->st_clonedfrom != 0)
		pst = state_with_serialno(st->st_clonedfrom);

	/* pads things up to message size boundary */
	{
		size_t blocksize = pst->st_oakley.encrypter->enc_blocksize;
		char  *b = alloca(blocksize);
		unsigned int i;
		size_t padding =  pad_up(pbs_offset(e_pbs_cipher), blocksize);
		if (padding == 0)
			padding = blocksize;

		for (i = 0; i < padding; i++)
			b[i] = i;
		out_raw(b, padding, e_pbs_cipher, "padding and length");
	}
}
