stf_status process_informational_ikev2(struct msg_digest *md)
{
	/* verify that there is in fact an encrypted payload */
	if (!md->chain[ISAKMP_NEXT_v2E]) {
		libreswan_log(
			"Ignoring informational exchange outside encrypted payload (rfc5996 section 1.4)");
		return STF_IGNORE;
	}

	/* decrypt things. */
	{
		stf_status ret;

		if (md->hdr.isa_flags & ISAKMP_FLAGS_I) {
			DBG(DBG_CONTROLMORE,
			    DBG_log(
				    "received informational exchange request from INITIATOR"));
			ret = ikev2_decrypt_msg(md, RESPONDER);
		} else {
			DBG(DBG_CONTROLMORE,
			    DBG_log(
				    "received informational exchange request from RESPONDER"));
			ret = ikev2_decrypt_msg(md, INITIATOR);
		}

		if (ret != STF_OK)
			return ret;
	}

	{
		struct payload_digest *p;
		struct ikev2_delete *v2del = NULL;
		stf_status ret;
		struct state *const st = md->st;

		/* Only send response if it is request*/
		if (!(md->hdr.isa_flags & ISAKMP_FLAGS_R)) {
			unsigned char *authstart;
			pb_stream e_pbs, e_pbs_cipher;
			struct ikev2_generic e;
			unsigned char *iv;
			int ivsize;
			unsigned char *encstart;

			/* beginning of data going out */
			authstart = reply_stream.cur;

			/* make sure HDR is at start of a clean buffer */
			zero(reply_buffer);
			init_pbs(&reply_stream, reply_buffer,
				 sizeof(reply_buffer),
				 "information exchange reply packet");

			DBG(DBG_CONTROLMORE | DBG_DPD,
			    DBG_log("Received an INFORMATIONAL request, "
				    "updating liveness, no longer pending"));
			st->st_last_liveness = now();
			st->st_pend_liveness = FALSE;

			/* HDR out */
			{
				struct isakmp_hdr r_hdr;
				zero(&r_hdr); /* default to 0 */  /* AAA should we copy from MD? */
				r_hdr.isa_version = build_ike_version();
				memcpy(r_hdr.isa_rcookie, st->st_rcookie,
				       COOKIE_SIZE);
				memcpy(r_hdr.isa_icookie, st->st_icookie,
				       COOKIE_SIZE);
				r_hdr.isa_xchg = ISAKMP_v2_INFORMATIONAL;
				r_hdr.isa_np = ISAKMP_NEXT_v2E;
				r_hdr.isa_msgid = htonl(md->msgid_received);

				/*set initiator bit if we are initiator*/
				if (md->role == INITIATOR)
					r_hdr.isa_flags |= ISAKMP_FLAGS_I;

				r_hdr.isa_flags  |=  ISAKMP_FLAGS_R;

				if (!out_struct(&r_hdr, &isakmp_hdr_desc,
						&reply_stream, &md->rbody)) {
					libreswan_log(
						"error initializing hdr for informational message");
					return STF_INTERNAL_ERROR;
				}

			} /*HDR Done*/

			/* insert an Encryption payload header */
			if (md->chain[ISAKMP_NEXT_v2D]) {
				bool ikesa_flag = FALSE;
				/* Search if there is a IKE SA delete payload*/
				for (p = md->chain[ISAKMP_NEXT_v2D]; p != NULL;
				     p = p->next) {
					if (p->payload.v2delete.isad_protoid ==
					    PROTO_ISAKMP) {
						e.isag_np = ISAKMP_NEXT_v2NONE;
						ikesa_flag = TRUE;
						break;
					}
				}
				/* if there is no IKE SA DELETE PAYLOAD*/
				/* That means, there are AH OR ESP*/
				if (!ikesa_flag)
					e.isag_np = ISAKMP_NEXT_v2D;

			} else {
				e.isag_np = ISAKMP_NEXT_v2NONE;
			}

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
			init_pbs(&e_pbs_cipher, e_pbs.cur,
				 e_pbs.roof - e_pbs.cur, "cleartext");
			e_pbs_cipher.container = &e_pbs;
			e_pbs_cipher.desc = NULL;
			e_pbs_cipher.cur = e_pbs.cur;
			encstart = e_pbs_cipher.cur;

			if (md->chain[ISAKMP_NEXT_v2D]) {

				for (p = md->chain[ISAKMP_NEXT_v2D]; p != NULL;
				     p = p->next) {
					v2del = &p->payload.v2delete;

					switch (v2del->isad_protoid) {
					case PROTO_ISAKMP:
						/* My understanding is that delete payload for IKE SA
						 *  should be the only payload in the informational exchange
						 */
						break;

					case PROTO_IPSEC_AH:
					case PROTO_IPSEC_ESP:
					{
						char spi_buf[1024];
						pb_stream del_pbs;
						struct ikev2_delete v2del_tmp;
						u_int16_t i, j = 0;
						u_char *spi;

						for (i = 0;
						     i < v2del->isad_nrspi;
						     i++ ) {
							spi = p->pbs.cur +
							      (i *
							       v2del->
							       isad_spisize);
							DBG(DBG_CONTROLMORE, DBG_log(
								    "received delete request for %s SA(0x%08lx)",
								    enum_show(
									    &
									    protocol_names,
									    v2del
									    ->
									    isad_protoid),
								    (
									    unsigned
									    long)
								    ntohl((
										  unsigned
										  long)
									  *(
										  ipsec_spi_t
										  *)
									  spi)));

							struct state *dst =
								find_state_ikev2_child_to_delete(
									st->st_icookie,
									st->st_rcookie,
									v2del->isad_protoid,
									*(
										ipsec_spi_t
										*)spi);

							if (dst != NULL) {
								struct
								ipsec_proto_info
								*pr =
									v2del->
									isad_protoid
									==
									PROTO_IPSEC_AH
									?
									&dst
									->st_ah
									:
									&dst
									->
									st_esp;
								DBG(
									DBG_CONTROLMORE,
									DBG_log(
										"our side spi that needs to be sent: %s SA(0x%08lx)",
										enum_show(
											&
											protocol_names,
											v2del
											->
											isad_protoid),
										(
											unsigned
											long)
										ntohl(
											pr
											->
											our_spi)));

								memcpy(
									spi_buf +
									(j *
									 v2del
									 ->
									 isad_spisize),
									(u_char
									 *)&pr->our_spi,
									v2del->isad_spisize);
								j++;
							} else {
								DBG(
									DBG_CONTROLMORE,
									DBG_log(
										"received delete request for %s SA(0x%08lx) but local state is not found",
										enum_show(
											&
											protocol_names,
											v2del
											->
											isad_protoid),
										(
											unsigned
											long)
										ntohl((
											      unsigned
											      long)
										      *(
											      ipsec_spi_t
											      *)
										      spi)));
							}
						}

						if ( !j ) {
							DBG(DBG_CONTROLMORE, DBG_log(
								    "This delete payload does not contain a single spi that has any local state, ignoring"));
							return STF_IGNORE;
						} else {
							DBG(DBG_CONTROLMORE, DBG_log(
								    "No. of SPIs to be sent %d",
								    j);
							    DBG_dump(
								    " Emit SPIs",
								    spi_buf,
								    j *
								    v2del->
								    isad_spisize));
						}

						zero(&v2del_tmp);

						if (p->next != NULL)
							v2del_tmp.isad_np =
								ISAKMP_NEXT_v2D;


						else
							v2del_tmp.isad_np =
								ISAKMP_NEXT_v2NONE;


						v2del_tmp.isad_protoid =
							v2del->isad_protoid;
						v2del_tmp.isad_spisize =
							v2del->isad_spisize;
						v2del_tmp.isad_nrspi = j;

						/* Emit delete payload header out*/
						if (!out_struct(&v2del_tmp,
								&
								ikev2_delete_desc,
								&e_pbs_cipher,
								&del_pbs)) {
							libreswan_log(
								"error initializing hdr for delete payload");
							return
								STF_INTERNAL_ERROR;
						}

						/* Emit values of spi to be sent to the peer*/
						if (!out_raw(spi_buf, j *
							     v2del->
							     isad_spisize,
							     &del_pbs,
							     "local spis")) {
							libreswan_log(
								"error sending spi values in delete payload");
							return
								STF_INTERNAL_ERROR;
						}

						close_output_pbs(&del_pbs);

					}
					break;
					default:
						/*Unrecongnized protocol */
						return STF_IGNORE;
					}

					/* this will break from for loop*/
					if (v2del->isad_protoid ==
					    PROTO_ISAKMP)
						break;

				}
			}

			/*If there are no payloads or in other words empty payload in request
			 * that means it is check for liveliness, so send an empty payload message
			 * this will end up sending an empty payload
			 */

			ikev2_padup_pre_encrypt(md, &e_pbs_cipher);
			close_output_pbs(&e_pbs_cipher);

			{
				unsigned char *authloc = ikev2_authloc(md,
								       &e_pbs);
				if (authloc == NULL)
					return STF_INTERNAL_ERROR;

				close_output_pbs(&e_pbs);
				close_output_pbs(&md->rbody);
				close_output_pbs(&reply_stream);

				ret = ikev2_encrypt_msg(md, md->role,
							authstart,
							iv, encstart, authloc,
							&e_pbs, &e_pbs_cipher);
				if (ret != STF_OK)
					return ret;
			}


			/* keep it for a retransmit if necessary */
			freeanychunk(st->st_tpacket);
			clonetochunk(st->st_tpacket, reply_stream.start, pbs_offset(
					     &reply_stream),
				     "reply packet for informational exchange");

			send_ike_msg(st, __FUNCTION__);
		}

		/* Now carry out the actualy task, we can not carry the actual task since
		 * we need to send informational responde using existig SAs
		 */

		{
			if (md->chain[ISAKMP_NEXT_v2D] && st->st_state !=
			    STATE_IKESA_DEL) {

				for (p = md->chain[ISAKMP_NEXT_v2D]; p != NULL;
				     p = p->next) {
					v2del = &p->payload.v2delete;

					switch (v2del->isad_protoid) {
					case PROTO_ISAKMP:
					{
						/* My understanding is that delete payload for IKE SA
						 *  should be the only payload in the informational
						 * Now delete the IKE SA state and all its child states
						 */
						struct state *current_st = st;
						struct state *next_st = NULL;
						struct state *first_st = NULL;

						/* Find the first state in the hash chain*/
						while (current_st !=
						       (struct state *) NULL) {
							first_st = current_st;
							current_st =
								first_st->
								st_hashchain_prev;
						}

						current_st = first_st;
						while (current_st !=
						       (struct state *) NULL) {
							next_st =
								current_st->
								st_hashchain_next;
							if (current_st->
							    st_clonedfrom !=
							    0 ) {
								change_state(
									current_st,
									STATE_CHILDSA_DEL);
							} else {
								change_state(
									current_st,
									STATE_IKESA_DEL);
							}
							delete_state(current_st);
							current_st = next_st;
						}
					}
					break;

					case PROTO_IPSEC_AH:
					case PROTO_IPSEC_ESP:
					{
						/* pb_stream del_pbs; */
						struct ikev2_delete;
						u_int16_t i;
						u_char *spi;

						for (i = 0;
						     i < v2del->isad_nrspi;
						     i++ ) {
							spi = p->pbs.cur +
							      (i *
							       v2del->
							       isad_spisize);
							DBG(DBG_CONTROLMORE, DBG_log(
								    "Now doing actual deletion for request: %s SA(0x%08lx)",
								    enum_show(
									    &
									    protocol_names,
									    v2del
									    ->
									    isad_protoid),
								    (
									    unsigned
									    long)
								    ntohl((
										  unsigned
										  long)
									  *(
										  ipsec_spi_t
										  *)
									  spi)));

							struct state *dst =
								find_state_ikev2_child_to_delete(
									st->st_icookie,
									st->st_rcookie,
									v2del->isad_protoid,
									*(
										ipsec_spi_t
										*)spi);

							if (dst != NULL) {
								struct
								ipsec_proto_info
								*pr =
									v2del->
									isad_protoid
									==
									PROTO_IPSEC_AH
									?
									&dst
									->st_ah
									:
									&dst
									->
									st_esp;
								DBG(
									DBG_CONTROLMORE,
									DBG_log(
										"our side spi that needs to be deleted: %s SA(0x%08lx)",
										enum_show(
											&
											protocol_names,
											v2del
											->
											isad_protoid),
										(
											unsigned
											long)
										ntohl(
											pr
											->
											our_spi)));

								/* now delete the state*/
								change_state(
									dst,
									STATE_CHILDSA_DEL);
								delete_state(
									dst);
							} else {
								DBG(
									DBG_CONTROLMORE,
									DBG_log(
										"received delete request for %s SA(0x%08lx) but local state is not found",
										enum_show(
											&
											protocol_names,
											v2del
											->
											isad_protoid),
										(
											unsigned
											long)
										ntohl((
											      unsigned
											      long)
										      *(
											      ipsec_spi_t
											      *)
										      spi)));
							}
						}
					}
					break;

					default:
						/*Unrecongnized protocol */
						return STF_IGNORE;
					}

					/* this will break from for loop*/
					if (v2del->isad_protoid ==
					    PROTO_ISAKMP)
						break;

				}       /* for */

			}               /* if*/
			else {
				/* empty response to our IKESA delete request*/
				if ((md->hdr.isa_flags & ISAKMP_FLAGS_R) &&
				    st->st_state == STATE_IKESA_DEL) {
					/* My understanding is that delete payload for IKE SA
					 *  should be the only payload in the informational
					 * Now delete the IKE SA state and all its child states
					 */
					struct state *current_st = st;
					struct state *next_st = NULL;
					struct state *first_st = NULL;

					/* Find the first state in the hash chain*/
					while (current_st !=
					       (struct state *) NULL) {
						first_st = current_st;
						current_st =
							first_st->
							st_hashchain_prev;
					}

					current_st = first_st;
					while (current_st !=
					       (struct state *) NULL) {
						next_st =
							current_st->
							st_hashchain_next;
						if (current_st->st_clonedfrom
						    !=
						    0 ) {
							change_state(
								current_st,
								STATE_CHILDSA_DEL);
						} else {
							change_state(
								current_st,
								STATE_IKESA_DEL);
						}
						delete_state(current_st);
						current_st = next_st;
					}
					/* empty response to our empty INFORMATIONAL
					 * We don't send anything back */
				} else if ((md->hdr.isa_flags &
					    ISAKMP_FLAGS_R) &&
					   st->st_state != STATE_IKESA_DEL) {
					DBG(DBG_CONTROLMORE,
					    DBG_log(
						    "Received an INFORMATIONAL response, "
						    "updating liveness, no longer pending."));
					st->st_last_liveness = now();
					st->st_pend_liveness = FALSE;
					st->st_msgid_lastrecv =
						md->msgid_received;
				}
			}
		}

	}

	return STF_OK;
}
