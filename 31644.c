static struct pattern *pat_match_meth(struct sample *smp, struct pattern_expr *expr, int fill)
{
	int icase;
	struct pattern_list *lst;
	struct pattern *pattern;

	list_for_each_entry(lst, &expr->patterns, list) {
		pattern = &lst->pat;

		/* well-known method */
		if (pattern->val.i != HTTP_METH_OTHER) {
			if (smp->data.u.meth.meth == pattern->val.i)
				return pattern;
			else
				continue;
		}

		/* Other method, we must compare the strings */
		if (pattern->len != smp->data.u.meth.str.len)
			continue;

		icase = expr->mflags & PAT_MF_IGNORE_CASE;
		if ((icase && strncasecmp(pattern->ptr.str, smp->data.u.meth.str.str, smp->data.u.meth.str.len) == 0) ||
		    (!icase && strncmp(pattern->ptr.str, smp->data.u.meth.str.str, smp->data.u.meth.str.len) == 0))
			return pattern;
	}
	return NULL;
}
