stf_status ikev2parent_outI1(int whack_sock,
			     struct connection *c,
			     struct state *predecessor,
			     lset_t policy,
			     unsigned long try,
			     enum crypto_importance importance
#ifdef HAVE_LABELED_IPSEC
			     , struct xfrm_user_sec_ctx_ike * uctx
#endif
			     )
{
	struct state *st = new_state();
	struct db_sa *sadb;
	int groupnum;
	int policy_index = POLICY_ISAKMP(policy,
					 c->spd.this.xauth_server,
					 c->spd.this.xauth_client);

	/* set up new state */
	get_cookie(TRUE, st->st_icookie, COOKIE_SIZE, &c->spd.that.host_addr);
	initialize_new_state(st, c, policy, try, whack_sock, importance);
	st->st_ikev2 = TRUE;
	change_state(st, STATE_PARENT_I1);
	st->st_msgid_lastack = INVALID_MSGID;
	st->st_msgid_nextuse = 0;
	st->st_try   = try;

	if (HAS_IPSEC_POLICY(policy)) {
#ifdef HAVE_LABELED_IPSEC
		st->sec_ctx = NULL;
		if ( uctx != NULL)
			libreswan_log(
				"Labeled ipsec is not supported with ikev2 yet");


#endif

		add_pending(dup_any(
				    whack_sock), st, c, policy, 1,
			    predecessor == NULL ? SOS_NOBODY : predecessor->st_serialno
#ifdef HAVE_LABELED_IPSEC
			    , st->sec_ctx
#endif
			    );
	}

	if (predecessor == NULL)
		libreswan_log("initiating v2 parent SA");
	else
		libreswan_log("initiating v2 parent SA to replace #%lu",
			      predecessor->st_serialno);

	if (predecessor != NULL) {
		update_pending(predecessor, st);
		whack_log(RC_NEW_STATE + STATE_PARENT_I1,
			  "%s: initiate, replacing #%lu",
			  enum_name(&state_names, st->st_state),
			  predecessor->st_serialno);
	} else {
		whack_log(RC_NEW_STATE + STATE_PARENT_I1,
			  "%s: initiate",
			  enum_name(&state_names, st->st_state));
	}

	/*
	 * now, we need to initialize st->st_oakley, specifically, the group
	 * number needs to be initialized.
	 */
	groupnum = 0;

	st->st_sadb = &oakley_sadb[policy_index];
	sadb = oakley_alg_makedb(st->st_connection->alg_info_ike,
				 st->st_sadb, 0);
	if (sadb != NULL)
		st->st_sadb = sadb;
	sadb = st->st_sadb = sa_v2_convert(st->st_sadb);
	{
		unsigned int pc_cnt;

		/* look at all the proposals */
		if (st->st_sadb->prop_disj != NULL) {
			for (pc_cnt = 0;
			     pc_cnt < st->st_sadb->prop_disj_cnt && groupnum ==
			     0;
			     pc_cnt++) {
				struct db_v2_prop *vp =
					&st->st_sadb->prop_disj[pc_cnt];
				unsigned int pr_cnt;

				/* look at all the proposals */
				if (vp->props != NULL) {
					for (pr_cnt = 0;
					     pr_cnt < vp->prop_cnt &&
					     groupnum == 0;
					     pr_cnt++) {
						unsigned int ts_cnt;
						struct db_v2_prop_conj *vpc =
							&vp->props[pr_cnt];

						for (ts_cnt = 0;
						     ts_cnt < vpc->trans_cnt &&
						     groupnum == 0; ts_cnt++) {
							struct db_v2_trans *tr
								=
									&vpc->
									trans[
										ts_cnt
									];
							if (tr != NULL &&
							    tr->transform_type
							    ==
							    IKEv2_TRANS_TYPE_DH)
							{
								groupnum =
									tr->
									transid;
							}
						}
					}
				}
			}
		}
	}
	if (groupnum == 0)
		groupnum = OAKLEY_GROUP_MODP2048;
	st->st_oakley.group = lookup_group(groupnum);
	st->st_oakley.groupnum = groupnum;

	/* now. we need to go calculate the nonce, and the KE */
	{
		struct ke_continuation *ke = alloc_thing(
			struct ke_continuation,
			"ikev2_outI1 KE");
		stf_status e;

		ke->md = alloc_md();
		ke->md->from_state = STATE_IKEv2_BASE;
		ke->md->svm = ikev2_parent_firststate();
		ke->md->st = st;
		set_suspended(st, ke->md);

		if (!st->st_sec_in_use) {
			pcrc_init(&ke->ke_pcrc);
			ke->ke_pcrc.pcrc_func = ikev2_parent_outI1_continue;
			e = build_ke(&ke->ke_pcrc, st, st->st_oakley.group,
				     importance);
			if ( (e != STF_SUSPEND &&
			      e != STF_INLINE) || (e == STF_TOOMUCHCRYPTO)) {
				loglog(RC_CRYPTOFAILED,
				       "system too busy - Enabling dcookies [TODO]");
				delete_state(st);
			}
		} else {
			e =
				ikev2_parent_outI1_tail(
					(struct pluto_crypto_req_cont *)ke,
					NULL);
		}

		reset_globals();

		return e;
	}
}
