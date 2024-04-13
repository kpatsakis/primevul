match_records(struct ftrace_hash *hash, char *buff,
	      int len, char *mod, int not)
{
	unsigned search_len = 0;
	struct ftrace_page *pg;
	struct dyn_ftrace *rec;
	int type = MATCH_FULL;
	char *search = buff;
	int found = 0;
	int ret;

	if (len) {
		type = filter_parse_regex(buff, len, &search, &not);
		search_len = strlen(search);
	}

	mutex_lock(&ftrace_lock);

	if (unlikely(ftrace_disabled))
		goto out_unlock;

	do_for_each_ftrace_rec(pg, rec) {
		if (ftrace_match_record(rec, mod, search, search_len, type)) {
			ret = enter_record(hash, rec, not);
			if (ret < 0) {
				found = ret;
				goto out_unlock;
			}
			found = 1;
		}
	} while_for_each_ftrace_rec();
 out_unlock:
	mutex_unlock(&ftrace_lock);

	return found;
}
