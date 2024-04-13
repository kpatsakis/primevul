stf_status ikev2parent_inR1outI2(struct msg_digest *md)
{
	struct state *st = md->st;
	/* struct connection *c = st->st_connection; */
	pb_stream *keyex_pbs;

	/* check if the responder replied with v2N with DOS COOKIE */
	if ( md->chain[ISAKMP_NEXT_v2N] &&
	     md->chain[ISAKMP_NEXT_v2N]->payload.v2n.isan_type ==
	     v2N_COOKIE) {
		u_int8_t spisize;
		const pb_stream *dc_pbs;
		DBG(DBG_CONTROLMORE,
		    DBG_log(
			    "inR1OutI2 received a DOS v2N_COOKIE from the responder");
		    DBG_log("resend the I1 with a cookie payload"));
		spisize = md->chain[ISAKMP_NEXT_v2N]->payload.v2n.isan_spisize;
		dc_pbs = &md->chain[ISAKMP_NEXT_v2N]->pbs;
		clonetochunk(st->st_dcookie,  (dc_pbs->cur + spisize),
			     (pbs_left(
				      dc_pbs) - spisize),
			     "saved received dcookie");

		DBG(DBG_CONTROLMORE,
		    DBG_dump_chunk("dcookie received (instead of a R1):",
				   st->st_dcookie);
		    DBG_log("next STATE_PARENT_I1 resend I1 with the dcookie"));

		md->svm = ikev2_parent_firststate();

		change_state(st, STATE_PARENT_I1);
		st->st_msgid_lastack = INVALID_MSGID;
		md->msgid_received = INVALID_MSGID; /* AAA hack  */
		st->st_msgid_nextuse = 0;

		return ikev2_parent_outI1_common(md, st);
	}

	/*
	 * If we did not get a KE payload, we cannot continue. There * should be
	 * a Notify telling us why. We inform the user, but continue to try this
	 * connection via regular retransmit intervals.
	 */
	if ( md->chain[ISAKMP_NEXT_v2N]  &&
	     (md->chain[ISAKMP_NEXT_v2KE] == NULL)) {
		const char *from_state_name = enum_name(&state_names,
							st->st_state);
		const u_int16_t isan_type =
			md->chain[ISAKMP_NEXT_v2N]->payload.v2n.isan_type;
		libreswan_log("%s: received %s",
			      from_state_name,
			      enum_name(&ikev2_notify_names, isan_type));
		return STF_FAIL + isan_type;
	} else if ( md->chain[ISAKMP_NEXT_v2N]) {
		DBG(DBG_CONTROL, DBG_log("received a notify.."));
	}

	/*
	 * the responder sent us back KE, Gr, Nr, and it's our time to calculate
	 * the shared key values.
	 */

	DBG(DBG_CONTROLMORE,
	    DBG_log(
		    "ikev2 parent inR1: calculating g^{xy} in order to send I2"));

	/* KE in */
	keyex_pbs = &md->chain[ISAKMP_NEXT_v2KE]->pbs;
	RETURN_STF_FAILURE(accept_KE(&st->st_gr, "Gr", st->st_oakley.group,
				     keyex_pbs));

	/* Ni in */
	RETURN_STF_FAILURE(accept_v2_nonce(md, &st->st_nr, "Ni"));

	if (md->chain[ISAKMP_NEXT_v2SA] == NULL) {
		libreswan_log("No responder SA proposal found");
		return v2N_INVALID_SYNTAX;
	}

	/* process and confirm the SA selected */
	{
		struct payload_digest *const sa_pd =
			md->chain[ISAKMP_NEXT_v2SA];
		v2_notification_t rn;

		/* SA body in and out */
		rn = ikev2_parse_parent_sa_body(&sa_pd->pbs,
						&sa_pd->payload.v2sa,
						NULL, st, FALSE);

		if (rn != v2N_NOTHING_WRONG)
			return STF_FAIL + rn;
	}

	/* update state */
	ikev2_update_counters(md);

	/* now. we need to go calculate the g^xy */
	{
		struct dh_continuation *dh = alloc_thing(
			struct dh_continuation,
			"ikev2_inR1outI2 KE");
		stf_status e;

		dh->md = md;
		set_suspended(st, dh->md);

		pcrc_init(&dh->dh_pcrc);
		dh->dh_pcrc.pcrc_func = ikev2_parent_inR1outI2_continue;
		e = start_dh_v2(&dh->dh_pcrc, st, st->st_import, INITIATOR,
				st->st_oakley.groupnum);
		if (e != STF_SUSPEND && e != STF_INLINE) {
			loglog(RC_CRYPTOFAILED, "system too busy");
			delete_state(st);
		}

		reset_globals();

		return e;
	}
}
