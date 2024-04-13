struct kern_ipc_perm *ipcctl_pre_down(struct ipc_namespace *ns,
				      struct ipc_ids *ids, int id, int cmd,
				      struct ipc64_perm *perm, int extra_perm)
{
	struct kern_ipc_perm *ipcp;

	ipcp = ipcctl_pre_down_nolock(ns, ids, id, cmd, perm, extra_perm);
	if (IS_ERR(ipcp))
		goto out;

	spin_lock(&ipcp->lock);
out:
	return ipcp;
}
