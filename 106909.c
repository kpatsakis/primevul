static stf_status ikev2_parent_outI1_common(struct msg_digest *md,
					    struct state *st)
{
	struct connection *c = st->st_connection;
	int numvidtosend = 0;

	/* set up reply */
	init_pbs(&reply_stream, reply_buffer, sizeof(reply_buffer),
		 "reply packet");

	/* HDR out */
	{
		struct isakmp_hdr hdr;

		zero(&hdr);                             /* default to 0 */
		/* Impair function will raise major/minor by 1 for testing */
		hdr.isa_version = build_ike_version();
		if (st->st_dcookie.ptr)
			hdr.isa_np   = ISAKMP_NEXT_v2N;
		else
			hdr.isa_np   = ISAKMP_NEXT_v2SA;
		hdr.isa_xchg = ISAKMP_v2_SA_INIT;
		hdr.isa_flags = ISAKMP_FLAGS_I;
		memcpy(hdr.isa_icookie, st->st_icookie, COOKIE_SIZE);
		/* R-cookie, are left zero */

		if (!out_struct(&hdr, &isakmp_hdr_desc, &reply_stream,
				&md->rbody)) {
			reset_cur_state();
			return STF_INTERNAL_ERROR;
		}
	}
	/* send an anti DOS cookie, 4306 2.6, if we have received one from the
	 * responder
	 */

	if (st->st_dcookie.ptr) {
		chunk_t child_spi;
		memset(&child_spi, 0, sizeof(child_spi));
		ship_v2N(ISAKMP_NEXT_v2SA, DBGP(
				 IMPAIR_SEND_BOGUS_ISAKMP_FLAG) ?
			 (ISAKMP_PAYLOAD_NONCRITICAL |
			  ISAKMP_PAYLOAD_LIBRESWAN_BOGUS) :
			 ISAKMP_PAYLOAD_NONCRITICAL, PROTO_ISAKMP,
			 &child_spi,
			 v2N_COOKIE, &st->st_dcookie, &md->rbody);
	}
	/* SA out */
	{
		u_char *sa_start = md->rbody.cur;

		if (st->st_sadb->prop_disj_cnt == 0 || st->st_sadb->prop_disj)
			st->st_sadb = sa_v2_convert(st->st_sadb);

		if (!ikev2_out_sa(&md->rbody,
				  PROTO_ISAKMP,
				  st->st_sadb,
				  st, TRUE, /* parentSA */
				  ISAKMP_NEXT_v2KE)) {
			libreswan_log("outsa fail");
			reset_cur_state();
			return STF_INTERNAL_ERROR;
		}
		/* save initiator SA for later HASH */
		if (st->st_p1isa.ptr == NULL) { /* no leak!  (MUST be first time) */
			clonetochunk(st->st_p1isa, sa_start,
				     md->rbody.cur - sa_start,
				     "sa in main_outI1");
		}
	}

	/* send KE */
	if (!justship_v2KE(st, &st->st_gi, st->st_oakley.groupnum,  &md->rbody,
			   ISAKMP_NEXT_v2Ni))
		return STF_INTERNAL_ERROR;

	/*
	 * Check which Vendor ID's we need to send - there will be more soon
	 * In IKEv2, DPD and NAT-T are no longer vendorid's
	 */
	if (c->send_vendorid) {
		numvidtosend++;  /* if we need to send Libreswan VID */
	}

	/* send NONCE */
	{
		int np = numvidtosend > 0 ? ISAKMP_NEXT_v2V : ISAKMP_NEXT_v2NONE;
		struct ikev2_generic in;
		pb_stream pb;

		memset(&in, 0, sizeof(in));
		in.isag_np = np;
		in.isag_critical = ISAKMP_PAYLOAD_NONCRITICAL;
		if (DBGP(IMPAIR_SEND_BOGUS_ISAKMP_FLAG)) {
			libreswan_log(
				" setting bogus ISAKMP_PAYLOAD_LIBRESWAN_BOGUS flag in ISAKMP payload");
			in.isag_critical |= ISAKMP_PAYLOAD_LIBRESWAN_BOGUS;
		}

		if (!out_struct(&in, &ikev2_nonce_desc, &md->rbody, &pb) ||
		    !out_raw(st->st_ni.ptr, st->st_ni.len, &pb, "IKEv2 nonce"))
			return STF_INTERNAL_ERROR;

		close_output_pbs(&pb);
	}

	/* Send Vendor VID if needed */
	if (c->send_vendorid) {
		const char *myvid = ipsec_version_vendorid();
		int np = --numvidtosend >
			 0 ? ISAKMP_NEXT_v2V : ISAKMP_NEXT_v2NONE;

		if (!out_generic_raw(np, &isakmp_vendor_id_desc, &md->rbody,
				     myvid, strlen(myvid),
				     "Vendor ID"))
			return STF_INTERNAL_ERROR;

		/* ensure our VID chain was valid */
		passert(numvidtosend == 0);
	}

	close_message(&md->rbody, st);
	close_output_pbs(&reply_stream);

	freeanychunk(st->st_tpacket);
	clonetochunk(st->st_tpacket, reply_stream.start,
		     pbs_offset(&reply_stream),
		     "reply packet for ikev2_parent_outI1_tail");

	/* save packet for later signing */
	freeanychunk(st->st_firstpacket_me);
	clonetochunk(st->st_firstpacket_me, reply_stream.start,
		     pbs_offset(&reply_stream), "saved first packet");

	/* Transmit */
	send_ike_msg(st, __FUNCTION__);

	delete_event(st);
	event_schedule(EVENT_v2_RETRANSMIT, EVENT_RETRANSMIT_DELAY_0, st);

	reset_cur_state();
	return STF_OK;
}
