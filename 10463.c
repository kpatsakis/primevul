static inline void path_it_deinit(struct path_it *it)
{
	struct list_head *skip, *tmp;
	/*
	 * The skip_list is used only for the MIN_INFLIGHT policy.
	 * We need to remove paths from it, so that next IO can insert
	 * paths (->mp_skip_entry) into a skip_list again.
	 */
	list_for_each_safe(skip, tmp, &it->skip_list)
		list_del_init(skip);
}