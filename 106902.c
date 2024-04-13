void ikev2_delete_out(struct state *st)
{
	struct state *pst = NULL;

	if (st->st_clonedfrom != 0) {
		/*child SA*/
		pst = state_with_serialno(st->st_clonedfrom);

		if (!pst) {
			DBG(DBG_CONTROL,
			    DBG_log("IKE SA does not exist for this child SA"));
			DBG(DBG_CONTROL,
			    DBG_log(
				    "INFORMATIONAL exchange can not be sent, deleting state"));
			goto end;
		}
	} else {
		/* Parent SA*/
		pst = st;

	}

	{
		unsigned char *authstart;
		pb_stream e_pbs, e_pbs_cipher;
		pb_stream rbody;
		struct ikev2_generic e;
		unsigned char *iv;
		int ivsize;
		unsigned char *encstart;
		struct msg_digest md;
		enum phase1_role role;

		md.st = st;
		md.pst = pst;
		/* beginning of data going out */
		authstart = reply_stream.cur;

		/* make sure HDR is at start of a clean buffer */
		zero(reply_buffer);
		init_pbs(&reply_stream, reply_buffer, sizeof(reply_buffer),
			 "information exchange request packet");

		/* HDR out */
		{
			struct isakmp_hdr r_hdr;
			zero(&r_hdr); /* default to 0 */  /* AAA should we copy from MD? */
			r_hdr.isa_version = build_ike_version();
			memcpy(r_hdr.isa_rcookie, pst->st_rcookie,
			       COOKIE_SIZE);
			memcpy(r_hdr.isa_icookie, pst->st_icookie,
			       COOKIE_SIZE);
			r_hdr.isa_xchg = ISAKMP_v2_INFORMATIONAL;
			r_hdr.isa_np = ISAKMP_NEXT_v2E;
			r_hdr.isa_msgid = htonl(pst->st_msgid_nextuse);

			/*set initiator bit if we are initiator*/
			if (pst->st_state == STATE_PARENT_I2 ||
			    pst->st_state == STATE_PARENT_I3) {
				r_hdr.isa_flags |= ISAKMP_FLAGS_I;
				role = INITIATOR;
			} else {
				role = RESPONDER;
			}

			/* r_hdr.isa_flags  |=  ISAKMP_FLAGS_R; */

			if (!out_struct(&r_hdr, &isakmp_hdr_desc,
					&reply_stream, &rbody)) {
				libreswan_log(
					"error initializing hdr for informational message");
				goto end;
			}

		} /*HDR Done*/

		/* insert an Encryption payload header */
		e.isag_np = ISAKMP_NEXT_v2D;
		e.isag_critical = ISAKMP_PAYLOAD_NONCRITICAL;

		if (!out_struct(&e, &ikev2_e_desc, &rbody, &e_pbs))
			goto end;

		/* insert IV */
		iv     = e_pbs.cur;
		ivsize = pst->st_oakley.encrypter->iv_size;
		if (!out_zero(ivsize, &e_pbs, "iv"))
			goto end;
		get_rnd_bytes(iv, ivsize);

		/* note where cleartext starts */
		init_pbs(&e_pbs_cipher, e_pbs.cur, e_pbs.roof - e_pbs.cur,
			 "cleartext");
		e_pbs_cipher.container = &e_pbs;
		e_pbs_cipher.desc = NULL;
		e_pbs_cipher.cur = e_pbs.cur;
		encstart = e_pbs_cipher.cur;

		{
			pb_stream del_pbs;
			struct ikev2_delete v2del_tmp;
			/*
			 * u_int16_t i, j=0;
			 * u_char *spi;
			 * char spi_buf[1024];
			 */

			zero(&v2del_tmp);
			v2del_tmp.isad_np = ISAKMP_NEXT_v2NONE;

			if (st->st_clonedfrom != 0 ) {
				v2del_tmp.isad_protoid = PROTO_IPSEC_ESP;
				v2del_tmp.isad_spisize = sizeof(ipsec_spi_t);
				v2del_tmp.isad_nrspi = 1;
			} else {
				v2del_tmp.isad_protoid = PROTO_ISAKMP;
				v2del_tmp.isad_spisize = 0;
				v2del_tmp.isad_nrspi = 0;
			}

			/* Emit delete payload header out*/
			if (!out_struct(&v2del_tmp, &ikev2_delete_desc,
					&e_pbs_cipher, &del_pbs)) {
				libreswan_log(
					"error initializing hdr for delete payload");
				goto end;
			}

			/* Emit values of spi to be sent to the peer*/
			if (st->st_clonedfrom != 0) {
				if (!out_raw( (u_char *)&st->st_esp.our_spi,
					      sizeof(ipsec_spi_t), &del_pbs,
					      "local spis")) {
					libreswan_log(
						"error sending spi values in delete payload");
					goto end;
				}
			}

			close_output_pbs(&del_pbs);

		}

		ikev2_padup_pre_encrypt(&md, &e_pbs_cipher);
		close_output_pbs(&e_pbs_cipher);

		{
			stf_status ret;
			unsigned char *authloc = ikev2_authloc(&md, &e_pbs);
			if (authloc == NULL)
				goto end;
			close_output_pbs(&e_pbs);
			close_output_pbs(&rbody);
			close_output_pbs(&reply_stream);

			ret = ikev2_encrypt_msg(&md, role,
						authstart,
						iv, encstart, authloc,
						&e_pbs, &e_pbs_cipher);
			if (ret != STF_OK)
				goto end;
		}

		/* keep it for a retransmit if necessary */
		freeanychunk(pst->st_tpacket);
		clonetochunk(pst->st_tpacket, reply_stream.start,
			     pbs_offset(&reply_stream),
			     "request packet for informational exchange");

		send_ike_msg(pst, __FUNCTION__);

		/* update state */
		ikev2_update_counters(&md);

	}

	/* If everything is fine, and we sent packet, goto real_end*/
	goto real_end;

end:
	/* If some error occurs above that prevents us sending a request packet*/
	/* delete the states right now*/

	if (st->st_clonedfrom != SOS_NOBODY) {
		change_state(st, STATE_CHILDSA_DEL);
		delete_state(st);
	} else {

		struct state *current_st = pst;
		struct state *next_st = NULL;
		struct state *first_st = NULL;

		/* Find the first state in the hash chain*/
		while (current_st != (struct state *) NULL) {
			first_st = current_st;
			current_st = first_st->st_hashchain_prev;
		}

		current_st = first_st;
		while (current_st != (struct state *) NULL) {
			next_st = current_st->st_hashchain_next;
			if (current_st->st_clonedfrom != 0 )
				change_state(current_st, STATE_CHILDSA_DEL);
			else
				change_state(current_st, STATE_IKESA_DEL);
			delete_state(current_st);
			current_st = next_st;
		}
	}

real_end:;
}
