static int netlbl_cipsov4_list(struct sk_buff *skb, struct genl_info *info)
{
	int ret_val;
	struct sk_buff *ans_skb = NULL;
	u32 nlsze_mult = 1;
	void *data;
	u32 doi;
	struct nlattr *nla_a;
	struct nlattr *nla_b;
	struct cipso_v4_doi *doi_def;
	u32 iter;

	if (!info->attrs[NLBL_CIPSOV4_A_DOI]) {
		ret_val = -EINVAL;
		goto list_failure;
	}

list_start:
	ans_skb = nlmsg_new(NLMSG_DEFAULT_SIZE * nlsze_mult, GFP_KERNEL);
	if (ans_skb == NULL) {
		ret_val = -ENOMEM;
		goto list_failure;
	}
	data = genlmsg_put_reply(ans_skb, info, &netlbl_cipsov4_gnl_family,
				 0, NLBL_CIPSOV4_C_LIST);
	if (data == NULL) {
		ret_val = -ENOMEM;
		goto list_failure;
	}

	doi = nla_get_u32(info->attrs[NLBL_CIPSOV4_A_DOI]);

	rcu_read_lock();
	doi_def = cipso_v4_doi_getdef(doi);
	if (doi_def == NULL) {
		ret_val = -EINVAL;
		goto list_failure;
	}

	ret_val = nla_put_u32(ans_skb, NLBL_CIPSOV4_A_MTYPE, doi_def->type);
	if (ret_val != 0)
		goto list_failure_lock;

	nla_a = nla_nest_start(ans_skb, NLBL_CIPSOV4_A_TAGLST);
	if (nla_a == NULL) {
		ret_val = -ENOMEM;
		goto list_failure_lock;
	}
	for (iter = 0;
	     iter < CIPSO_V4_TAG_MAXCNT &&
	       doi_def->tags[iter] != CIPSO_V4_TAG_INVALID;
	     iter++) {
		ret_val = nla_put_u8(ans_skb,
				     NLBL_CIPSOV4_A_TAG,
				     doi_def->tags[iter]);
		if (ret_val != 0)
			goto list_failure_lock;
	}
	nla_nest_end(ans_skb, nla_a);

	switch (doi_def->type) {
	case CIPSO_V4_MAP_STD:
		nla_a = nla_nest_start(ans_skb, NLBL_CIPSOV4_A_MLSLVLLST);
		if (nla_a == NULL) {
			ret_val = -ENOMEM;
			goto list_failure_lock;
		}
		for (iter = 0;
		     iter < doi_def->map.std->lvl.local_size;
		     iter++) {
			if (doi_def->map.std->lvl.local[iter] ==
			    CIPSO_V4_INV_LVL)
				continue;

			nla_b = nla_nest_start(ans_skb, NLBL_CIPSOV4_A_MLSLVL);
			if (nla_b == NULL) {
				ret_val = -ENOMEM;
				goto list_retry;
			}
			ret_val = nla_put_u32(ans_skb,
					      NLBL_CIPSOV4_A_MLSLVLLOC,
					      iter);
			if (ret_val != 0)
				goto list_retry;
			ret_val = nla_put_u32(ans_skb,
					    NLBL_CIPSOV4_A_MLSLVLREM,
					    doi_def->map.std->lvl.local[iter]);
			if (ret_val != 0)
				goto list_retry;
			nla_nest_end(ans_skb, nla_b);
		}
		nla_nest_end(ans_skb, nla_a);

		nla_a = nla_nest_start(ans_skb, NLBL_CIPSOV4_A_MLSCATLST);
		if (nla_a == NULL) {
			ret_val = -ENOMEM;
			goto list_retry;
		}
		for (iter = 0;
		     iter < doi_def->map.std->cat.local_size;
		     iter++) {
			if (doi_def->map.std->cat.local[iter] ==
			    CIPSO_V4_INV_CAT)
				continue;

			nla_b = nla_nest_start(ans_skb, NLBL_CIPSOV4_A_MLSCAT);
			if (nla_b == NULL) {
				ret_val = -ENOMEM;
				goto list_retry;
			}
			ret_val = nla_put_u32(ans_skb,
					      NLBL_CIPSOV4_A_MLSCATLOC,
					      iter);
			if (ret_val != 0)
				goto list_retry;
			ret_val = nla_put_u32(ans_skb,
					    NLBL_CIPSOV4_A_MLSCATREM,
					    doi_def->map.std->cat.local[iter]);
			if (ret_val != 0)
				goto list_retry;
			nla_nest_end(ans_skb, nla_b);
		}
		nla_nest_end(ans_skb, nla_a);

		break;
	}
	rcu_read_unlock();

	genlmsg_end(ans_skb, data);

	ret_val = genlmsg_reply(ans_skb, info);
	if (ret_val != 0)
		goto list_failure;

	return 0;

list_retry:
	/* XXX - this limit is a guesstimate */
	if (nlsze_mult < 4) {
		rcu_read_unlock();
		kfree_skb(ans_skb);
		nlsze_mult++;
		goto list_start;
	}
list_failure_lock:
	rcu_read_unlock();
list_failure:
	kfree_skb(ans_skb);
	return ret_val;
}
