static stf_status ikev2_encrypt_msg(struct msg_digest *md,
				    enum phase1_role init,
				    unsigned char *authstart,
				    unsigned char *iv,
				    unsigned char *encstart,
				    unsigned char *authloc,
				    pb_stream *e_pbs UNUSED,
				    pb_stream *e_pbs_cipher)
{
	struct state *st = md->st;
	struct state *pst = st;
	chunk_t *cipherkey, *authkey;

	if (st->st_clonedfrom != 0)
		pst = state_with_serialno(st->st_clonedfrom);

	if (init == INITIATOR) {
		cipherkey = &pst->st_skey_ei;
		authkey   = &pst->st_skey_ai;
	} else {
		cipherkey = &pst->st_skey_er;
		authkey   = &pst->st_skey_ar;
	}

	/* encrypt the block */
	{
		size_t blocksize = pst->st_oakley.encrypter->enc_blocksize;
		unsigned char *savediv = alloca(blocksize);
		unsigned int cipherlen = e_pbs_cipher->cur - encstart;

		DBG(DBG_CRYPT,
		    DBG_dump("data before encryption:", encstart, cipherlen));

		memcpy(savediv, iv, blocksize);

		/* now, encrypt */
		(st->st_oakley.encrypter->do_crypt)(encstart,
						    cipherlen,
						    cipherkey->ptr,
						    cipherkey->len,
						    savediv, TRUE);

		DBG(DBG_CRYPT,
		    DBG_dump("data after encryption:", encstart, cipherlen));
	}

	/* okay, authenticate from beginning of IV */
	{
		struct hmac_ctx ctx;
		DBG(DBG_PARSING, DBG_log("Inside authloc"));
		DBG(DBG_CRYPT,
		    DBG_dump("authkey value: ", authkey->ptr, authkey->len));
		hmac_init_chunk(&ctx, pst->st_oakley.integ_hasher, *authkey);
		DBG(DBG_PARSING, DBG_log("Inside authloc after init"));
		hmac_update(&ctx, authstart, authloc - authstart);
		DBG(DBG_PARSING, DBG_log("Inside authloc after update"));
		hmac_final(authloc, &ctx);
		DBG(DBG_PARSING, DBG_log("Inside authloc after final"));

		DBG(DBG_PARSING, {
			    DBG_dump("data being hmac:", authstart, authloc -
				     authstart);
			    DBG_dump("out calculated auth:", authloc,
				     pst->st_oakley.integ_hasher->
				     hash_integ_len);
		    });
	}

	return STF_OK;
}
