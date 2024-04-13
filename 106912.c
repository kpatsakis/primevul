stf_status ikev2_send_informational(struct state *st)
{
	struct state *pst = NULL;

	if (st->st_clonedfrom != SOS_NOBODY) {
		pst = state_with_serialno(st->st_clonedfrom);
		if (!pst) {
			DBG(DBG_CONTROL,
			    DBG_log(
				    "IKE SA does not exist for this child SA - should not happen"));
			DBG(DBG_CONTROL,
			    DBG_log("INFORMATIONAL exchange can not be sent"));
			return STF_IGNORE;
		}
	} else {
		pst = st;
	}

	{
		unsigned char *authstart;
		unsigned char *encstart;
		unsigned char *iv;
		int ivsize;
		struct msg_digest md;
		struct ikev2_generic e;
		enum phase1_role role;
		pb_stream e_pbs, e_pbs_cipher;
		pb_stream rbody;
		pb_stream request;
		u_char buffer[1024];

		md.st = st;
		md.pst = pst;
		memset(buffer, 0, sizeof(buffer));
		init_pbs(&request, buffer, sizeof(buffer),
			 "informational exchange request packet");
		authstart = request.cur;

		/* HDR out */
		{
			struct isakmp_hdr r_hdr;
			zero(&r_hdr);
			r_hdr.isa_version = build_ike_version();
			memcpy(r_hdr.isa_rcookie, pst->st_rcookie,
			       COOKIE_SIZE);
			memcpy(r_hdr.isa_icookie, pst->st_icookie,
			       COOKIE_SIZE);
			r_hdr.isa_xchg = ISAKMP_v2_INFORMATIONAL;
			r_hdr.isa_np = ISAKMP_NEXT_v2E;

			if (pst->st_state == STATE_PARENT_I2 ||
			    pst->st_state == STATE_PARENT_I3) {
				r_hdr.isa_flags |= ISAKMP_FLAGS_I;
				role = INITIATOR;
				r_hdr.isa_msgid = htonl(pst->st_msgid_nextuse);
			} else {
				role = RESPONDER;
				r_hdr.isa_msgid = htonl(
					pst->st_msgid_lastrecv + 1);
			}

			if (!out_struct(&r_hdr, &isakmp_hdr_desc,
					&request, &rbody)) {
				libreswan_log(
					"error initializing hdr for informational message");
				return STF_FATAL;
			}
		} /* HDR done*/

		/* insert an Encryption payload header */
		e.isag_np = ISAKMP_NEXT_v2NONE;
		e.isag_critical = ISAKMP_PAYLOAD_NONCRITICAL;
		if (!out_struct(&e, &ikev2_e_desc, &rbody, &e_pbs))
			return STF_FATAL;

		/* IV */
		iv = e_pbs.cur;
		ivsize = pst->st_oakley.encrypter->iv_size;
		if (!out_zero(ivsize, &e_pbs, "iv"))
			return STF_FATAL;

		get_rnd_bytes(iv, ivsize);

		/* note where cleartext starts */
		init_pbs(&e_pbs_cipher, e_pbs.cur, e_pbs.roof - e_pbs.cur,
			 "cleartext");
		e_pbs_cipher.container = &e_pbs;
		e_pbs_cipher.desc = NULL;
		e_pbs_cipher.cur = e_pbs.cur;
		encstart = e_pbs_cipher.cur;

		/* This is an empty informational exchange (A.K.A liveness check) */
		ikev2_padup_pre_encrypt(&md, &e_pbs_cipher);
		close_output_pbs(&e_pbs_cipher);

		{
			stf_status ret;
			unsigned char *authloc = ikev2_authloc(&md, &e_pbs);

			if (!authloc)
				return STF_FATAL;

			close_output_pbs(&e_pbs);
			close_output_pbs(&rbody);
			close_output_pbs(&request);

			ret = ikev2_encrypt_msg(&md, role,
						authstart,
						iv, encstart, authloc,
						&e_pbs, &e_pbs_cipher);
			if (ret != STF_OK)
				return STF_FATAL;
		}
		/* keep it for a retransmit if necessary */
		freeanychunk(pst->st_tpacket);
		clonetochunk(pst->st_tpacket, request.start,
			     pbs_offset(&request),
			     "reply packet for informational exchange");
		pst->st_pend_liveness = TRUE; /* we should only do this when dpd/liveness is active? */
		send_ike_msg(pst, __FUNCTION__);
		ikev2_update_counters(&md);

	}

	return STF_OK;
}
