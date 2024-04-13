void dput(struct dentry *dentry)
{
	if (unlikely(!dentry))
		return;

repeat:
	rcu_read_lock();
	if (likely(fast_dput(dentry))) {
		rcu_read_unlock();
		return;
	}

	/* Slow case: now with the dentry lock held */
	rcu_read_unlock();

	/* Unreachable? Get rid of it */
	if (unlikely(d_unhashed(dentry)))
		goto kill_it;

	if (unlikely(dentry->d_flags & DCACHE_DISCONNECTED))
		goto kill_it;

	if (unlikely(dentry->d_flags & DCACHE_OP_DELETE)) {
		if (dentry->d_op->d_delete(dentry))
			goto kill_it;
	}

	if (!(dentry->d_flags & DCACHE_REFERENCED))
		dentry->d_flags |= DCACHE_REFERENCED;
	dentry_lru_add(dentry);

	dentry->d_lockref.count--;
	spin_unlock(&dentry->d_lock);
	return;

kill_it:
	dentry = dentry_kill(dentry);
	if (dentry)
		goto repeat;
}
