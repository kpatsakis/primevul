 static stf_status ikev2_parent_inI2outR2_tail(
	struct pluto_crypto_req_cont *pcrc,
	struct pluto_crypto_req *r)
{
	struct dh_continuation *dh = (struct dh_continuation *)pcrc;
	struct msg_digest *md  = dh->md;
	struct state *const st = md->st;
	struct connection *c   = st->st_connection;
	unsigned char *idhash_in, *idhash_out;
	unsigned char *authstart;
	unsigned int np;
	int v2_notify_num = 0;

	/* extract calculated values from r */
	finish_dh_v2(st, r);

	if (DBGP(DBG_PRIVATE) && DBGP(DBG_CRYPT))
		ikev2_log_parentSA(st);

	/* decrypt things. */
	{
		stf_status ret;
		ret = ikev2_decrypt_msg(md, RESPONDER);
		if (ret != STF_OK)
			return ret;
	}

	/*Once the message has been decrypted, then only we can check for auth payload*/
	/*check the presense of auth payload now so that it does not crash in rehash_state if auth payload has not been received*/
	if (!md->chain[ISAKMP_NEXT_v2AUTH]) {
		libreswan_log("no authentication payload found");
		return STF_FAIL;
	}

	if (!ikev2_decode_peer_id(md, RESPONDER))
		return STF_FAIL + v2N_AUTHENTICATION_FAILED;

	{
		struct hmac_ctx id_ctx;
		const pb_stream *id_pbs = &md->chain[ISAKMP_NEXT_v2IDi]->pbs;
		unsigned char *idstart = id_pbs->start + 4;
		unsigned int idlen  = pbs_room(id_pbs) - 4;

		hmac_init_chunk(&id_ctx, st->st_oakley.prf_hasher,
				st->st_skey_pi);

		/* calculate hash of IDi for AUTH below */
		DBG(DBG_CRYPT,
		    DBG_dump_chunk("idhash verify pi", st->st_skey_pi));
		DBG(DBG_CRYPT, DBG_dump("idhash verify I2", idstart, idlen));
		hmac_update(&id_ctx, idstart, idlen);
		idhash_in = alloca(st->st_oakley.prf_hasher->hash_digest_len);
		hmac_final(idhash_in, &id_ctx);
	}

	/* process CERT payload */
	{
		if (md->chain[ISAKMP_NEXT_v2CERT]) {
			/* should we check if we should accept a cert payload ?
			 *  has_preloaded_public_key(st)
			 */
			DBG(DBG_CONTROLMORE,
			    DBG_log(
				    "has a v2_CERT payload going to process it "));
			ikev2_decode_cert(md);
		}
	}

	/* process CERTREQ payload */
	if (md->chain[ISAKMP_NEXT_v2CERTREQ]) {
		DBG(DBG_CONTROLMORE,
		    DBG_log("has a v2CERTREQ payload going to decode it"));
		ikev2_decode_cr(md, &st->st_connection->requested_ca);
	}

	/* process AUTH payload now */
	/* now check signature from RSA key */
	switch (md->chain[ISAKMP_NEXT_v2AUTH]->payload.v2a.isaa_type) {
	case v2_AUTH_RSA:
	{
		stf_status authstat = ikev2_verify_rsa_sha1(st,
							    RESPONDER,
							    idhash_in,
							    NULL,       /* keys from DNS */
							    NULL,       /* gateways from DNS */
							    &md->chain[
								    ISAKMP_NEXT_v2AUTH]->pbs);
		if (authstat != STF_OK) {
			libreswan_log("RSA authentication failed");
			SEND_NOTIFICATION(v2N_AUTHENTICATION_FAILED);
			return STF_FATAL;
		}
		break;
	}
	case v2_AUTH_SHARED:
	{
		stf_status authstat = ikev2_verify_psk_auth(st,
							    RESPONDER,
							    idhash_in,
							    &md->chain[
								    ISAKMP_NEXT_v2AUTH]->pbs);
		if (authstat != STF_OK) {
			libreswan_log(
				"PSK authentication failed AUTH mismatch!");
			SEND_NOTIFICATION(v2N_AUTHENTICATION_FAILED);
			return STF_FATAL;
		}
		break;
	}
	default:
		libreswan_log("authentication method: %s not supported",
			      enum_name(&ikev2_auth_names,
					md->chain[ISAKMP_NEXT_v2AUTH]->payload.
					v2a.isaa_type));
		return STF_FATAL;
	}

	/* Is there a notify about an error ? */
	if (md->chain[ISAKMP_NEXT_v2N] != NULL) {
		DBG(DBG_CONTROL,
		    DBG_log(
			    " notify payload detected, should be processed...."));
	}

	/* good. now create child state */
	/* note: as we will switch to child state, we force the parent to the
	 * new state now */
	change_state(st, STATE_PARENT_R2);
	c->newest_isakmp_sa = st->st_serialno;

	delete_event(st);
	event_schedule(EVENT_SA_REPLACE, c->sa_ike_life_seconds, st);

	authstart = reply_stream.cur;
	/* send response */
	{
		unsigned char *encstart;
		unsigned char *iv;
		unsigned int ivsize;
		struct ikev2_generic e;
		pb_stream e_pbs, e_pbs_cipher;
		stf_status ret;
		bool send_cert = FALSE;

		/* make sure HDR is at start of a clean buffer */
		zero(reply_buffer);
		init_pbs(&reply_stream, reply_buffer, sizeof(reply_buffer),
			 "reply packet");

		/* HDR out */
		{
			struct isakmp_hdr r_hdr = md->hdr;

			r_hdr.isa_np    = ISAKMP_NEXT_v2E;
			r_hdr.isa_xchg  = ISAKMP_v2_AUTH;
			r_hdr.isa_flags = ISAKMP_FLAGS_R;
			memcpy(r_hdr.isa_icookie, st->st_icookie, COOKIE_SIZE);
			memcpy(r_hdr.isa_rcookie, st->st_rcookie, COOKIE_SIZE);
			if (!out_struct(&r_hdr, &isakmp_hdr_desc,
					&reply_stream, &md->rbody))
				return STF_INTERNAL_ERROR;
		}

		/* insert an Encryption payload header */
		e.isag_np = ISAKMP_NEXT_v2IDr;
		e.isag_critical = ISAKMP_PAYLOAD_NONCRITICAL;

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

		/* decide to send CERT payload before we generate IDr */
		send_cert = doi_send_ikev2_cert_thinking(st);

		/* send out the IDr payload */
		{
			struct ikev2_id r_id;
			pb_stream r_id_pbs;
			chunk_t id_b;
			struct hmac_ctx id_ctx;
			unsigned char *id_start;
			unsigned int id_len;

			hmac_init_chunk(&id_ctx, st->st_oakley.prf_hasher,
					st->st_skey_pr);
			build_id_payload((struct isakmp_ipsec_id *)&r_id,
					 &id_b,
					 &c->spd.this);
			r_id.isai_critical = ISAKMP_PAYLOAD_NONCRITICAL;

			if (send_cert)
				r_id.isai_np = ISAKMP_NEXT_v2CERT;
			else
				r_id.isai_np = ISAKMP_NEXT_v2AUTH;

			id_start = e_pbs_cipher.cur;

			if (!out_struct(&r_id,
					&ikev2_id_desc,
					&e_pbs_cipher,
					&r_id_pbs) ||
			    !out_chunk(id_b, &r_id_pbs, "my identity"))
				return STF_INTERNAL_ERROR;

			close_output_pbs(&r_id_pbs);

			id_start += 4;

			/* calculate hash of IDi for AUTH below */
			id_len = e_pbs_cipher.cur - id_start;
			DBG(DBG_CRYPT,
			    DBG_dump_chunk("idhash calc pr", st->st_skey_pr));
			DBG(DBG_CRYPT,
			    DBG_dump("idhash calc R2", id_start, id_len));
			hmac_update(&id_ctx, id_start, id_len);
			idhash_out = alloca(
				st->st_oakley.prf_hasher->hash_digest_len);
			hmac_final(idhash_out, &id_ctx);
		}

		DBG(DBG_CONTROLMORE,
		    DBG_log("assembled IDr payload -- CERT next"));

		/* send CERT payload RFC 4306 3.6, 1.2:([CERT,] ) */
		if (send_cert) {
			stf_status certstat = ikev2_send_cert(st, md,
							      RESPONDER,
							      ISAKMP_NEXT_v2AUTH,
							      &e_pbs_cipher);
			if (certstat != STF_OK)
				return certstat;
		}

		/* authentication good, see if there is a child SA being proposed */
		if (md->chain[ISAKMP_NEXT_v2SA] == NULL ||
		    md->chain[ISAKMP_NEXT_v2TSi] == NULL ||
		    md->chain[ISAKMP_NEXT_v2TSr] == NULL) {

			/* initiator didn't propose anything. Weird. Try unpending out end. */
			/* UNPEND XXX */
			libreswan_log("No CHILD SA proposals received.");
			np = ISAKMP_NEXT_v2NONE;
		} else {
			DBG_log("CHILD SA proposals received");
			libreswan_log(
				"PAUL: this is where we have to check the TSi/TSr");
			np = ISAKMP_NEXT_v2SA;
		}

		DBG(DBG_CONTROLMORE,
		    DBG_log("going to assemble AUTH payload"));

		/* now send AUTH payload */
		{
			stf_status authstat = ikev2_send_auth(c, st,
							      RESPONDER, np,
							      idhash_out,
							      &e_pbs_cipher);
			if (authstat != STF_OK)
				return authstat;
		}

		if (np == ISAKMP_NEXT_v2SA) {
			/* must have enough to build an CHILD_SA */
			ret = ikev2_child_sa_respond(md, RESPONDER,
						     &e_pbs_cipher);
			if (ret > STF_FAIL) {
				v2_notify_num = ret - STF_FAIL;
				DBG(DBG_CONTROL,
				    DBG_log(
					    "ikev2_child_sa_respond returned STF_FAIL with %s",
					    enum_name(&ikev2_notify_names,
						      v2_notify_num)));
				np = ISAKMP_NEXT_v2NONE;
			} else if (ret != STF_OK) {
				DBG_log("ikev2_child_sa_respond returned %s", enum_name(
						&stfstatus_name,
						ret));
				np = ISAKMP_NEXT_v2NONE;
			}
		}

		ikev2_padup_pre_encrypt(md, &e_pbs_cipher);
		close_output_pbs(&e_pbs_cipher);

		{
			unsigned char *authloc = ikev2_authloc(md, &e_pbs);

			if (authloc == NULL)
				return STF_INTERNAL_ERROR;

			close_output_pbs(&e_pbs);

			close_output_pbs(&md->rbody);
			close_output_pbs(&reply_stream);

			ret = ikev2_encrypt_msg(md, RESPONDER,
						authstart,
						iv, encstart, authloc,
						&e_pbs, &e_pbs_cipher);
			if (ret != STF_OK)
				return ret;
		}
	}

	/* keep it for a retransmit if necessary */
	freeanychunk(st->st_tpacket);
	clonetochunk(st->st_tpacket, reply_stream.start,
		     pbs_offset(&reply_stream),
		     "reply packet for ikev2_parent_inI2outR2_tail");

	/* note: retransimission is driven by initiator */

	/* if the child failed, delete its state here - we sent the packet */
	/* PAUL */
	return STF_OK;

}
