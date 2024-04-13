stf_status ikev2parent_inR2(struct msg_digest *md)
{
	struct state *st = md->st;
	struct connection *c = st->st_connection;
	unsigned char *idhash_in;
	struct state *pst = st;

	if (st->st_clonedfrom != 0)
		pst = state_with_serialno(st->st_clonedfrom);

	/*
	 * the initiator sent us an encrypted payload. We need to calculate
	 * our g^xy, and skeyseed values, and then decrypt the payload.
	 */

	DBG(DBG_CONTROLMORE,
	    DBG_log(
		    "ikev2 parent inR2: calculating g^{xy} in order to decrypt I2"));

	/* verify that there is in fact an encrypted payload */
	if (!md->chain[ISAKMP_NEXT_v2E]) {
		libreswan_log("R2 state should receive an encrypted payload");
		return STF_FATAL;
	}

	/* decrypt things. */
	{
		stf_status ret;
		ret = ikev2_decrypt_msg(md, INITIATOR);
		if (ret != STF_OK)
			return ret;
	}

	if (!ikev2_decode_peer_id(md, INITIATOR))
		return STF_FAIL + v2N_AUTHENTICATION_FAILED;

	{
		struct hmac_ctx id_ctx;
		const pb_stream *id_pbs = &md->chain[ISAKMP_NEXT_v2IDr]->pbs;
		unsigned char *idstart = id_pbs->start + 4;
		unsigned int idlen  = pbs_room(id_pbs) - 4;

		hmac_init_chunk(&id_ctx, pst->st_oakley.prf_hasher,
				pst->st_skey_pr);

		/* calculate hash of IDr for AUTH below */
		DBG(DBG_CRYPT,
		    DBG_dump_chunk("idhash verify pr", pst->st_skey_pr));
		DBG(DBG_CRYPT, DBG_dump("idhash auth R2", idstart, idlen));
		hmac_update(&id_ctx, idstart, idlen);
		idhash_in = alloca(pst->st_oakley.prf_hasher->hash_digest_len);
		hmac_final(idhash_in, &id_ctx);
	}

	if (md->chain[ISAKMP_NEXT_v2CERT]) {
		/* should we check if we should accept a cert payload ?
		 *  has_preloaded_public_key(st)
		 */
		/* in v1 code it is  decode_cert(struct msg_digest *md) */
		DBG(DBG_CONTROLMORE,
		    DBG_log("has a v2_CERT payload going to decode it"));
		ikev2_decode_cert(md);
	}

	/* process AUTH payload */
	if (!md->chain[ISAKMP_NEXT_v2AUTH]) {
		libreswan_log("no authentication payload found");
		return STF_FAIL;
	}

	/* now check signature from RSA key */
	switch (md->chain[ISAKMP_NEXT_v2AUTH]->payload.v2a.isaa_type) {
	case v2_AUTH_RSA:
	{
		stf_status authstat = ikev2_verify_rsa_sha1(pst,
							    INITIATOR,
							    idhash_in,
							    NULL,       /* keys from DNS */
							    NULL,       /* gateways from DNS */
							    &md->chain[
								    ISAKMP_NEXT_v2AUTH]->pbs);
		if (authstat != STF_OK) {
			libreswan_log("authentication failed");
			SEND_NOTIFICATION(v2N_AUTHENTICATION_FAILED);
			return STF_FAIL;
		}
		break;
	}
	case v2_AUTH_SHARED:
	{
		stf_status authstat = ikev2_verify_psk_auth(pst,
							    INITIATOR,
							    idhash_in,
							    &md->chain[
								    ISAKMP_NEXT_v2AUTH]->pbs);
		if (authstat != STF_OK) {
			libreswan_log("PSK authentication failed");
			SEND_NOTIFICATION(v2N_AUTHENTICATION_FAILED);
			return STF_FAIL;
		}
		break;
	}

	default:
		libreswan_log("authentication method: %s not supported",
			      enum_name(&ikev2_auth_names,
					md->chain[ISAKMP_NEXT_v2AUTH]->payload.
					v2a.isaa_type));
		return STF_FAIL;
	}

	/*
	 * update the parent state to make sure that it knows we have
	 * authenticated properly.
	 */
	change_state(pst, STATE_PARENT_I3);
	c->newest_isakmp_sa = pst->st_serialno;

	/* authentication good, see if there is a child SA available */
	if (md->chain[ISAKMP_NEXT_v2SA] == NULL ||
	    md->chain[ISAKMP_NEXT_v2TSi] == NULL ||
	    md->chain[ISAKMP_NEXT_v2TSr] == NULL) {
		/* not really anything to here... but it would be worth unpending again */
		DBG(DBG_CONTROLMORE,
		    DBG_log(
			    "no v2SA, v2TSi or v2TSr received, not attempting to setup child SA"));
		DBG(DBG_CONTROLMORE,
		    DBG_log("  Should we check for some notify?"));
		/*
		 * Delete previous retransmission event.
		 */
		delete_event(st);
		return STF_OK;
	}

	{
		int bestfit_n, bestfit_p, bestfit_pr;
		unsigned int best_tsi_i,  best_tsr_i;
		bestfit_n = -1;
		bestfit_p = -1;
		bestfit_pr = -1;

		/* Check TSi/TSr http://tools.ietf.org/html/rfc5996#section-2.9 */
		DBG(DBG_CONTROLMORE,
		    DBG_log(" check narrowing - we are responding to I2"));

		struct payload_digest *const tsi_pd =
			md->chain[ISAKMP_NEXT_v2TSi];
		struct payload_digest *const tsr_pd =
			md->chain[ISAKMP_NEXT_v2TSr];
		struct traffic_selector tsi[16], tsr[16];
#if 0
		bool instantiate = FALSE;
		ip_subnet tsi_subnet, tsr_subnet;
		const char *oops;
#endif

		unsigned int tsi_n, tsr_n;
		tsi_n = ikev2_parse_ts(tsi_pd, tsi, 16);
		tsr_n = ikev2_parse_ts(tsr_pd, tsr, 16);

		DBG_log(
			"Checking TSi(%d)/TSr(%d) selectors, looking for exact match", tsi_n,
			tsr_n);
		{
			struct spd_route *sra;
			sra = &c->spd;
			int bfit_n = ikev2_evaluate_connection_fit(c, sra,
								   INITIATOR,
								   tsi, tsr,
								   tsi_n,
								   tsr_n);
			if (bfit_n > bestfit_n) {
				DBG(DBG_CONTROLMORE,
				    DBG_log(
					    "bfit_n=ikev2_evaluate_connection_fit found better fit c %s",
					    c->name));
				int bfit_p =
					ikev2_evaluate_connection_port_fit(c,
									   sra,
									   INITIATOR,
									   tsi, tsr,
									   tsi_n, tsr_n, &best_tsi_i,
									   &best_tsr_i);
				if (bfit_p > bestfit_p) {
					DBG(DBG_CONTROLMORE,
					    DBG_log(
						    "ikev2_evaluate_connection_port_fit found better fit c %s, tsi[%d],tsr[%d]",
						    c->name,
						    best_tsi_i, best_tsr_i));
					int bfit_pr =
						ikev2_evaluate_connection_protocol_fit(
							c, sra, INITIATOR, tsi,
							tsr,
							tsi_n, tsr_n,
							&best_tsi_i,
							&best_tsr_i);
					if (bfit_pr > bestfit_pr ) {
						DBG(DBG_CONTROLMORE,
						    DBG_log(
							    "ikev2_evaluate_connection_protocol_fit found better fit c %s, tsi[%d],tsr[%d]",
							    c
							    ->name, best_tsi_i,
							    best_tsr_i));
						bestfit_p = bfit_p;
						bestfit_n = bfit_n;
					} else {
						DBG(DBG_CONTROLMORE,
						    DBG_log(
							    "protocol range fit c %s c->name was rejected by protocol matching",
							    c
							    ->name));
					}
				}
			} else {
				DBG(DBG_CONTROLMORE,
				    DBG_log(
					    "prefix range fit c %s c->name was rejected by port matching",
					    c->name));
			}
		}

		if ( ( bestfit_n > 0 )  && (bestfit_p > 0)) {
			DBG(DBG_CONTROLMORE,
			    DBG_log(
				    (
					    "found an acceptable TSi/TSr Traffic Selector")));
			memcpy(&st->st_ts_this, &tsi[best_tsi_i],
			       sizeof(struct traffic_selector));
			memcpy(&st->st_ts_that, &tsr[best_tsr_i],
			       sizeof(struct traffic_selector));
			ikev2_print_ts(&st->st_ts_this);
			ikev2_print_ts(&st->st_ts_that);

			ip_subnet tmp_subnet_i;
			ip_subnet tmp_subnet_r;
			rangetosubnet(&st->st_ts_this.low,
				      &st->st_ts_this.high, &tmp_subnet_i);
			rangetosubnet(&st->st_ts_that.low,
				      &st->st_ts_that.high, &tmp_subnet_r);

			c->spd.this.client = tmp_subnet_i;
			c->spd.this.port  = st->st_ts_this.startport;
			c->spd.this.protocol  = st->st_ts_this.ipprotoid;
			setportof(htons(
					  c->spd.this.port),
				  &c->spd.this.host_addr);
			setportof(htons(
					  c->spd.this.port),
				  &c->spd.this.client.addr);

			if ( subnetishost(&c->spd.this.client) &&
			     addrinsubnet(&c->spd.this.host_addr,
					  &c->spd.this.client))
				c->spd.this.has_client = FALSE;
			else
				c->spd.this.has_client = TRUE;

			c->spd.that.client = tmp_subnet_r;
			c->spd.that.port = st->st_ts_that.startport;
			c->spd.that.protocol = st->st_ts_that.ipprotoid;
			setportof(htons(
					  c->spd.that.port),
				  &c->spd.that.host_addr);
			setportof(htons(
					  c->spd.that.port),
				  &c->spd.that.client.addr);

			if ( subnetishost(&c->spd.that.client) &&
			     addrinsubnet(&c->spd.that.host_addr,
					  &c->spd.that.client))
				c->spd.that.has_client = FALSE;
			else
				c->spd.that.has_client = TRUE;
			/* AAAA */
		} else {
			DBG(DBG_CONTROLMORE,
			    DBG_log((
					    "reject responder TSi/TSr Traffic Selector")));
			/* prevents parent from going to I3 */
			return STF_FAIL + v2N_TS_UNACCEPTABLE;
		}
	} /* end of TS check block */

	{
		v2_notification_t rn;
		struct payload_digest *const sa_pd =
			md->chain[ISAKMP_NEXT_v2SA];

		rn = ikev2_parse_child_sa_body(&sa_pd->pbs,
					       &sa_pd->payload.v2sa,
					       NULL, st, FALSE);

		if (rn != v2N_NOTHING_WRONG)
			return STF_FAIL + rn;
	}

	{
		struct payload_digest *p;

		for (p = md->chain[ISAKMP_NEXT_v2N]; p != NULL; p = p->next) {
			/* RFC 5996 */
			/*Types in the range 0 - 16383 are intended for reporting errors.  An
			 * implementation receiving a Notify payload with one of these types
			 * that it does not recognize in a response MUST assume that the
			 * corresponding request has failed entirely.  Unrecognized error types
			 * in a request and status types in a request or response MUST be
			 * ignored, and they should be logged.*/

			if (enum_name(&ikev2_notify_names,
				      p->payload.v2n.isan_type) == NULL) {
				if (p->payload.v2n.isan_type <
				    v2N_INITIAL_CONTACT)
					return STF_FAIL +
					       p->payload.v2n.isan_type;
			}

			if ( p->payload.v2n.isan_type ==
			     v2N_USE_TRANSPORT_MODE ) {
				if ( st->st_connection->policy &
				     POLICY_TUNNEL) {
					/*This means we did not send v2N_USE_TRANSPORT, however responder is sending it in now (inR2), seems incorrect*/
					DBG(DBG_CONTROLMORE,
					    DBG_log(
						    "Initiator policy is tunnel, responder sends v2N_USE_TRANSPORT_MODE notification in inR2, ignoring it"));
				} else {
					DBG(DBG_CONTROLMORE,
					    DBG_log(
						    "Initiator policy is transport, responder sends v2N_USE_TRANSPORT_MODE, setting CHILD SA to transport mode"));
					if (st->st_esp.present == TRUE) {
						/*libreswan supports only "esp" with ikev2 it seems, look at ikev2_parse_child_sa_body handling*/
						st->st_esp.attrs.encapsulation
							=
								ENCAPSULATION_MODE_TRANSPORT;
					}
				}
			}
		}       /* for */

	}               /* notification block */

	ikev2_derive_child_keys(st, md->role);

	c->newest_ipsec_sa = st->st_serialno;

	/* now install child SAs */
	if (!install_ipsec_sa(st, TRUE))
		return STF_FATAL;

	/*
	 * Delete previous retransmission event.
	 */
	delete_event(st);

	return STF_OK;
}
