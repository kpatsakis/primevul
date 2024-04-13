static stf_status ikev2_parent_inR1outI2_tail(
	struct pluto_crypto_req_cont *pcrc,
	struct pluto_crypto_req *r)
{
	struct dh_continuation *dh = (struct dh_continuation *)pcrc;
	struct msg_digest *md = dh->md;
	struct state *st      = md->st;
	struct connection *c  = st->st_connection;
	struct ikev2_generic e;
	unsigned char *encstart;
	pb_stream e_pbs, e_pbs_cipher;
	unsigned char *iv;
	int ivsize;
	stf_status ret;
	unsigned char *idhash;
	unsigned char *authstart;
	struct state *pst = st;
	bool send_cert = FALSE;

	finish_dh_v2(st, r);

	if (DBGP(DBG_PRIVATE) && DBGP(DBG_CRYPT))
		ikev2_log_parentSA(st);

	pst = st;
	st = duplicate_state(pst);
	st->st_msgid = htonl(pst->st_msgid_nextuse); /* PAUL: note ordering */
	insert_state(st);
	md->st = st;
	md->pst = pst;

	/* parent had crypto failed, replace it with rekey! */
	delete_event(pst);
	event_schedule(EVENT_SA_REPLACE, c->sa_ike_life_seconds, pst);

	/* need to force parent state to I2 */
	change_state(pst, STATE_PARENT_I2);

	/* record first packet for later checking of signature */
	clonetochunk(pst->st_firstpacket_him, md->message_pbs.start,
		     pbs_offset(
			     &md->message_pbs), "saved first received packet");

	/* beginning of data going out */
	authstart = reply_stream.cur;

	/* make sure HDR is at start of a clean buffer */
	zero(reply_buffer);
	init_pbs(&reply_stream, reply_buffer, sizeof(reply_buffer),
		 "reply packet");

	/* HDR out */
	{
		struct isakmp_hdr r_hdr = md->hdr;

		r_hdr.isa_np    = ISAKMP_NEXT_v2E;
		r_hdr.isa_xchg  = ISAKMP_v2_AUTH;
		r_hdr.isa_flags = ISAKMP_FLAGS_I;
		r_hdr.isa_msgid = st->st_msgid;
		memcpy(r_hdr.isa_icookie, st->st_icookie, COOKIE_SIZE);
		memcpy(r_hdr.isa_rcookie, st->st_rcookie, COOKIE_SIZE);
		if (!out_struct(&r_hdr, &isakmp_hdr_desc, &reply_stream,
				&md->rbody))
			return STF_INTERNAL_ERROR;
	}

	/* insert an Encryption payload header */
	e.isag_np = ISAKMP_NEXT_v2IDi;
	e.isag_critical = ISAKMP_PAYLOAD_NONCRITICAL;
	if (DBGP(IMPAIR_SEND_BOGUS_ISAKMP_FLAG)) {
		libreswan_log(
			" setting bogus ISAKMP_PAYLOAD_LIBRESWAN_BOGUS flag in ISAKMP payload");
		e.isag_critical |= ISAKMP_PAYLOAD_LIBRESWAN_BOGUS;
	}

	if (!out_struct(&e, &ikev2_e_desc, &md->rbody, &e_pbs))
		return STF_INTERNAL_ERROR;

	/* insert IV */
	iv     = e_pbs.cur;
	ivsize = st->st_oakley.encrypter->iv_size;
	if (!out_zero(ivsize, &e_pbs, "iv"))
		return STF_INTERNAL_ERROR;

	get_rnd_bytes(iv, ivsize);

	/* note where cleartext starts */
	init_pbs(&e_pbs_cipher, e_pbs.cur, e_pbs.roof - e_pbs.cur,
		 "cleartext");
	e_pbs_cipher.container = &e_pbs;
	e_pbs_cipher.desc = NULL;
	e_pbs_cipher.cur = e_pbs.cur;
	encstart = e_pbs_cipher.cur;

	/* send out the IDi payload */
	{
		struct ikev2_id r_id;
		pb_stream r_id_pbs;
		chunk_t id_b;
		struct hmac_ctx id_ctx;
		unsigned char *id_start;
		unsigned int id_len;

		hmac_init_chunk(&id_ctx, pst->st_oakley.prf_hasher,
				pst->st_skey_pi);
		build_id_payload((struct isakmp_ipsec_id *)&r_id, &id_b,
				 &c->spd.this);
		r_id.isai_critical = ISAKMP_PAYLOAD_NONCRITICAL;
		if (DBGP(IMPAIR_SEND_BOGUS_ISAKMP_FLAG)) {
			libreswan_log(
				" setting bogus ISAKMP_PAYLOAD_LIBRESWAN_BOGUS flag in ISAKMP payload");
			r_id.isai_critical |= ISAKMP_PAYLOAD_LIBRESWAN_BOGUS;
		}

		{       /* decide to send CERT payload */
			send_cert = doi_send_ikev2_cert_thinking(st);

			if (send_cert)
				r_id.isai_np = ISAKMP_NEXT_v2CERT;
			else
				r_id.isai_np = ISAKMP_NEXT_v2AUTH;
		}

		id_start = e_pbs_cipher.cur;
		if (!out_struct(&r_id,
				&ikev2_id_desc,
				&e_pbs_cipher,
				&r_id_pbs) ||
		    !out_chunk(id_b, &r_id_pbs, "my identity"))
			return STF_INTERNAL_ERROR;

		/* HASH of ID is not done over common header */
		id_start += 4;

		close_output_pbs(&r_id_pbs);

		/* calculate hash of IDi for AUTH below */
		id_len = e_pbs_cipher.cur - id_start;
		DBG(DBG_CRYPT, DBG_dump_chunk("idhash calc pi",
					      pst->st_skey_pi));
		DBG(DBG_CRYPT, DBG_dump("idhash calc I2", id_start, id_len));
		hmac_update(&id_ctx, id_start, id_len);
		idhash = alloca(pst->st_oakley.prf_hasher->hash_digest_len);
		hmac_final(idhash, &id_ctx);
	}

	/* send [CERT,] payload RFC 4306 3.6, 1.2) */
	{

		if (send_cert) {
			stf_status certstat = ikev2_send_cert( st, md,
							       INITIATOR,
							       ISAKMP_NEXT_v2AUTH,
							       &e_pbs_cipher);
			if (certstat != STF_OK)
				return certstat;
		}
	}

	/* send out the AUTH payload */
	{
		lset_t policy;
		struct connection *c0 = first_pending(pst, &policy,
						      &st->st_whack_sock);
		unsigned int np = (c0 ? ISAKMP_NEXT_v2SA : ISAKMP_NEXT_v2NONE);
		DBG(DBG_CONTROL,
		    DBG_log(" payload after AUTH will be %s",
			    (c0) ? "ISAKMP_NEXT_v2SA" :
			    "ISAKMP_NEXT_v2NONE/NOTIFY"));

		stf_status authstat = ikev2_send_auth(c, st,
						      INITIATOR,
						      np,
						      idhash, &e_pbs_cipher);
		if (authstat != STF_OK)
			return authstat;

		/*
		 * now, find an eligible child SA from the pending list, and emit
		 * SA2i, TSi and TSr and (v2N_USE_TRANSPORT_MODE notification in transport mode) for it .
		 */
		if (c0) {
			chunk_t child_spi, notify_data;
			st->st_connection = c0;

			ikev2_emit_ipsec_sa(md, &e_pbs_cipher,
					    ISAKMP_NEXT_v2TSi, c0, policy);

			st->st_ts_this = ikev2_end_to_ts(&c0->spd.this);
			st->st_ts_that = ikev2_end_to_ts(&c0->spd.that);

			ikev2_calc_emit_ts(md, &e_pbs_cipher, INITIATOR, c0,
					   policy);

			if ( !(st->st_connection->policy & POLICY_TUNNEL) ) {
				DBG_log(
					"Initiator child policy is transport mode, sending v2N_USE_TRANSPORT_MODE");
				memset(&child_spi, 0, sizeof(child_spi));
				memset(&notify_data, 0, sizeof(notify_data));
				ship_v2N(ISAKMP_NEXT_v2NONE,
					 ISAKMP_PAYLOAD_NONCRITICAL, 0,
					 &child_spi,
					 v2N_USE_TRANSPORT_MODE, &notify_data,
					 &e_pbs_cipher);
			}
		} else {
			libreswan_log(
				"no pending SAs found, PARENT SA keyed only");
		}
	}

	/*
	 * need to extend the packet so that we will know how big it is
	 * since the length is under the integrity check
	 */
	ikev2_padup_pre_encrypt(md, &e_pbs_cipher);
	close_output_pbs(&e_pbs_cipher);

	{
		unsigned char *authloc = ikev2_authloc(md, &e_pbs);

		if (authloc == NULL)
			return STF_INTERNAL_ERROR;

		close_output_pbs(&e_pbs);
		close_output_pbs(&md->rbody);
		close_output_pbs(&reply_stream);

		ret = ikev2_encrypt_msg(md, INITIATOR,
					authstart,
					iv, encstart, authloc,
					&e_pbs, &e_pbs_cipher);
		if (ret != STF_OK)
			return ret;
	}

	/* keep it for a retransmit if necessary, but on initiator
	 * we never do that, but send_ike_msg() uses it.
	 */
	freeanychunk(pst->st_tpacket);
	clonetochunk(pst->st_tpacket, reply_stream.start,
		     pbs_offset(&reply_stream),
		     "reply packet for ikev2_parent_outI1");

	/*
	 * Delete previous retransmission event.
	 */
	delete_event(st);
	event_schedule(EVENT_v2_RETRANSMIT, EVENT_RETRANSMIT_DELAY_0, st);

	return STF_OK;

}
