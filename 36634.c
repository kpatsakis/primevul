void nfs4_schedule_state_manager(struct nfs_client *clp)
{
	struct task_struct *task;

	if (test_and_set_bit(NFS4CLNT_MANAGER_RUNNING, &clp->cl_state) != 0)
		return;
	__module_get(THIS_MODULE);
	atomic_inc(&clp->cl_count);
	task = kthread_run(nfs4_run_state_manager, clp, "%s-manager",
				rpc_peeraddr2str(clp->cl_rpcclient,
							RPC_DISPLAY_ADDR));
	if (!IS_ERR(task))
		return;
	nfs4_clear_state_manager_bit(clp);
	nfs_put_client(clp);
	module_put(THIS_MODULE);
}
