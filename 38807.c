static int nfs4_opendata_access(struct rpc_cred *cred,
				struct nfs4_opendata *opendata,
				struct nfs4_state *state, fmode_t fmode)
{
	struct nfs_access_entry cache;
	u32 mask;

	/* access call failed or for some reason the server doesn't
	 * support any access modes -- defer access call until later */
	if (opendata->o_res.access_supported == 0)
		return 0;

	mask = 0;
	/* don't check MAY_WRITE - a newly created file may not have
	 * write mode bits, but POSIX allows the creating process to write */
	if (fmode & FMODE_READ)
		mask |= MAY_READ;
	if (fmode & FMODE_EXEC)
		mask |= MAY_EXEC;

	cache.cred = cred;
	cache.jiffies = jiffies;
	nfs_access_set_mask(&cache, opendata->o_res.access_result);
	nfs_access_add_cache(state->inode, &cache);

	if ((mask & ~cache.mask & (MAY_READ | MAY_EXEC)) == 0)
		return 0;

	/* even though OPEN succeeded, access is denied. Close the file */
	nfs4_close_state(state, fmode);
	return -EACCES;
}
