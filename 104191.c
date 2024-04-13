file_magicfind(struct magic_set *ms, const char *name, struct mlist *v)
{
	uint32_t i, j;
	struct mlist *mlist, *ml;

	mlist = ms->mlist[1];

	for (ml = mlist->next; ml != mlist; ml = ml->next) {
		struct magic *ma = ml->magic;
		uint32_t nma = ml->nmagic;
		for (i = 0; i < nma; i++) {
			if (ma[i].type != FILE_NAME)
				continue;
			if (strcmp(ma[i].value.s, name) == 0) {
				v->magic = &ma[i];
				for (j = i + 1; j < nma; j++)
				    if (ma[j].cont_level == 0)
					    break;
				v->nmagic = j - i;
				return 0;
			}
		}
	}
	return -1;
}
