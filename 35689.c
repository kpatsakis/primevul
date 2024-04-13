int nfs4_proc_destroy_session(struct nfs4_session *session)
{
	int status = 0;
	struct rpc_message msg;

	dprintk("--> nfs4_proc_destroy_session\n");

	/* session is still being setup */
	if (session->clp->cl_cons_state != NFS_CS_READY)
		return status;

	msg.rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_DESTROY_SESSION];
	msg.rpc_argp = session;
	msg.rpc_resp = NULL;
	msg.rpc_cred = NULL;
	status = rpc_call_sync(session->clp->cl_rpcclient, &msg, RPC_TASK_TIMEOUT);

	if (status)
		printk(KERN_WARNING
			"NFS: Got error %d from the server on DESTROY_SESSION. "
			"Session has been destroyed regardless...\n", status);

	dprintk("<-- nfs4_proc_destroy_session\n");
	return status;
}
