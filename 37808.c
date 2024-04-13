static int do_update_counters(struct net *net, const char *name,
				struct ebt_counter __user *counters,
				unsigned int num_counters,
				const void __user *user, unsigned int len)
{
	int i, ret;
	struct ebt_counter *tmp;
	struct ebt_table *t;

	if (num_counters == 0)
		return -EINVAL;

	tmp = vmalloc(num_counters * sizeof(*tmp));
	if (!tmp)
		return -ENOMEM;

	t = find_table_lock(net, name, &ret, &ebt_mutex);
	if (!t)
		goto free_tmp;

	if (num_counters != t->private->nentries) {
		BUGPRINT("Wrong nr of counters\n");
		ret = -EINVAL;
		goto unlock_mutex;
	}

	if (copy_from_user(tmp, counters, num_counters * sizeof(*counters))) {
		ret = -EFAULT;
		goto unlock_mutex;
	}

	/* we want an atomic add of the counters */
	write_lock_bh(&t->lock);

	/* we add to the counters of the first cpu */
	for (i = 0; i < num_counters; i++) {
		t->private->counters[i].pcnt += tmp[i].pcnt;
		t->private->counters[i].bcnt += tmp[i].bcnt;
	}

	write_unlock_bh(&t->lock);
	ret = 0;
unlock_mutex:
	mutex_unlock(&ebt_mutex);
free_tmp:
	vfree(tmp);
	return ret;
}
