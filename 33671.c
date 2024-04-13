put_prentries(struct prentry *tentry, prentries *bulkentries)
{
    struct prlistentries *entry;

    if (bulkentries->prentries_val == 0) {
	bulkentries->prentries_len = 0;
	bulkentries->prentries_val =
	    (struct prlistentries *)malloc(PR_MAXENTRIES *
					   sizeof(struct prentry));
	if (!bulkentries->prentries_val) {
	    return (PRNOMEM);
	}
    }

    if (bulkentries->prentries_len >= PR_MAXENTRIES) {
	return (-1);
    }

    entry = (struct prlistentries *)bulkentries->prentries_val;
    entry += bulkentries->prentries_len;

    entry->flags = tentry->flags >> PRIVATE_SHIFT;
    if (entry->flags == 0) {
	entry->flags =
	    ((tentry->
	      flags & PRGRP) ? prp_group_default : prp_user_default) >>
	    PRIVATE_SHIFT;
    }
    entry->owner = tentry->owner;
    entry->id = tentry->id;
    entry->creator = tentry->creator;
    entry->ngroups = tentry->ngroups;
    entry->nusers = tentry->nusers;
    entry->count = tentry->count;
    strncpy(entry->name, tentry->name, PR_MAXNAMELEN);
    memset(entry->reserved, 0, sizeof(entry->reserved));
    bulkentries->prentries_len++;
    return 0;
}
