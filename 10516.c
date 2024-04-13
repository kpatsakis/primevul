static void rtrs_clt_remove_path_from_arr(struct rtrs_clt_path *clt_path)
{
	struct rtrs_clt_sess *clt = clt_path->clt;
	struct rtrs_clt_path *next;
	bool wait_for_grace = false;
	int cpu;

	mutex_lock(&clt->paths_mutex);
	list_del_rcu(&clt_path->s.entry);

	/* Make sure everybody observes path removal. */
	synchronize_rcu();

	/*
	 * At this point nobody sees @sess in the list, but still we have
	 * dangling pointer @pcpu_path which _can_ point to @sess.  Since
	 * nobody can observe @sess in the list, we guarantee that IO path
	 * will not assign @sess to @pcpu_path, i.e. @pcpu_path can be equal
	 * to @sess, but can never again become @sess.
	 */

	/*
	 * Decrement paths number only after grace period, because
	 * caller of do_each_path() must firstly observe list without
	 * path and only then decremented paths number.
	 *
	 * Otherwise there can be the following situation:
	 *    o Two paths exist and IO is coming.
	 *    o One path is removed:
	 *      CPU#0                          CPU#1
	 *      do_each_path():                rtrs_clt_remove_path_from_arr():
	 *          path = get_next_path()
	 *          ^^^                            list_del_rcu(path)
	 *          [!CONNECTED path]              clt->paths_num--
	 *                                              ^^^^^^^^^
	 *          load clt->paths_num                 from 2 to 1
	 *                    ^^^^^^^^^
	 *                    sees 1
	 *
	 *      path is observed as !CONNECTED, but do_each_path() loop
	 *      ends, because expression i < clt->paths_num is false.
	 */
	clt->paths_num--;

	/*
	 * Get @next connection from current @sess which is going to be
	 * removed.  If @sess is the last element, then @next is NULL.
	 */
	rcu_read_lock();
	next = list_next_or_null_rr_rcu(&clt->paths_list, &clt_path->s.entry,
					typeof(*next), s.entry);
	rcu_read_unlock();

	/*
	 * @pcpu paths can still point to the path which is going to be
	 * removed, so change the pointer manually.
	 */
	for_each_possible_cpu(cpu) {
		struct rtrs_clt_path __rcu **ppcpu_path;

		ppcpu_path = per_cpu_ptr(clt->pcpu_path, cpu);
		if (rcu_dereference_protected(*ppcpu_path,
			lockdep_is_held(&clt->paths_mutex)) != clt_path)
			/*
			 * synchronize_rcu() was called just after deleting
			 * entry from the list, thus IO code path cannot
			 * change pointer back to the pointer which is going
			 * to be removed, we are safe here.
			 */
			continue;

		/*
		 * We race with IO code path, which also changes pointer,
		 * thus we have to be careful not to overwrite it.
		 */
		if (xchg_paths(ppcpu_path, clt_path, next))
			/*
			 * @ppcpu_path was successfully replaced with @next,
			 * that means that someone could also pick up the
			 * @sess and dereferencing it right now, so wait for
			 * a grace period is required.
			 */
			wait_for_grace = true;
	}
	if (wait_for_grace)
		synchronize_rcu();

	mutex_unlock(&clt->paths_mutex);
}