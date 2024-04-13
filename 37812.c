ebt_check_entry_size_and_hooks(const struct ebt_entry *e,
   const struct ebt_table_info *newinfo,
   unsigned int *n, unsigned int *cnt,
   unsigned int *totalcnt, unsigned int *udc_cnt)
{
	int i;

	for (i = 0; i < NF_BR_NUMHOOKS; i++) {
		if ((void *)e == (void *)newinfo->hook_entry[i])
			break;
	}
	/* beginning of a new chain
	   if i == NF_BR_NUMHOOKS it must be a user defined chain */
	if (i != NF_BR_NUMHOOKS || !e->bitmask) {
		/* this checks if the previous chain has as many entries
		   as it said it has */
		if (*n != *cnt) {
			BUGPRINT("nentries does not equal the nr of entries "
				 "in the chain\n");
			return -EINVAL;
		}
		if (((struct ebt_entries *)e)->policy != EBT_DROP &&
		   ((struct ebt_entries *)e)->policy != EBT_ACCEPT) {
			/* only RETURN from udc */
			if (i != NF_BR_NUMHOOKS ||
			   ((struct ebt_entries *)e)->policy != EBT_RETURN) {
				BUGPRINT("bad policy\n");
				return -EINVAL;
			}
		}
		if (i == NF_BR_NUMHOOKS) /* it's a user defined chain */
			(*udc_cnt)++;
		if (((struct ebt_entries *)e)->counter_offset != *totalcnt) {
			BUGPRINT("counter_offset != totalcnt");
			return -EINVAL;
		}
		*n = ((struct ebt_entries *)e)->nentries;
		*cnt = 0;
		return 0;
	}
	/* a plain old entry, heh */
	if (sizeof(struct ebt_entry) > e->watchers_offset ||
	   e->watchers_offset > e->target_offset ||
	   e->target_offset >= e->next_offset) {
		BUGPRINT("entry offsets not in right order\n");
		return -EINVAL;
	}
	/* this is not checked anywhere else */
	if (e->next_offset - e->target_offset < sizeof(struct ebt_entry_target)) {
		BUGPRINT("target size too small\n");
		return -EINVAL;
	}
	(*cnt)++;
	(*totalcnt)++;
	return 0;
}
