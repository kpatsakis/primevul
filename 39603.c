ftrace_set_func(unsigned long *array, int *idx, char *buffer)
{
	struct dyn_ftrace *rec;
	struct ftrace_page *pg;
	int search_len;
	int fail = 1;
	int type, not;
	char *search;
	bool exists;
	int i;

	/* decode regex */
	type = filter_parse_regex(buffer, strlen(buffer), &search, &not);
	if (!not && *idx >= FTRACE_GRAPH_MAX_FUNCS)
		return -EBUSY;

	search_len = strlen(search);

	mutex_lock(&ftrace_lock);

	if (unlikely(ftrace_disabled)) {
		mutex_unlock(&ftrace_lock);
		return -ENODEV;
	}

	do_for_each_ftrace_rec(pg, rec) {

		if (ftrace_match_record(rec, NULL, search, search_len, type)) {
			/* if it is in the array */
			exists = false;
			for (i = 0; i < *idx; i++) {
				if (array[i] == rec->ip) {
					exists = true;
					break;
				}
			}

			if (!not) {
				fail = 0;
				if (!exists) {
					array[(*idx)++] = rec->ip;
					if (*idx >= FTRACE_GRAPH_MAX_FUNCS)
						goto out;
				}
			} else {
				if (exists) {
					array[i] = array[--(*idx)];
					array[*idx] = 0;
					fail = 0;
				}
			}
		}
	} while_for_each_ftrace_rec();
out:
	mutex_unlock(&ftrace_lock);

	if (fail)
		return -EINVAL;

	ftrace_graph_filter_enabled = 1;
	return 0;
}
