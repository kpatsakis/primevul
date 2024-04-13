int nfs4_proc_setclientid(struct nfs_client *clp, u32 program,
		unsigned short port, struct rpc_cred *cred,
		struct nfs4_setclientid_res *res)
{
	nfs4_verifier sc_verifier;
	struct nfs4_setclientid setclientid = {
		.sc_verifier = &sc_verifier,
		.sc_prog = program,
		.sc_cb_ident = clp->cl_cb_ident,
	};
	struct rpc_message msg = {
		.rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_SETCLIENTID],
		.rpc_argp = &setclientid,
		.rpc_resp = res,
		.rpc_cred = cred,
	};
	int loop = 0;
	int status;

	nfs4_construct_boot_verifier(clp, &sc_verifier);

	for(;;) {
		rcu_read_lock();
		setclientid.sc_name_len = scnprintf(setclientid.sc_name,
				sizeof(setclientid.sc_name), "%s/%s %s %s %u",
				clp->cl_ipaddr,
				rpc_peeraddr2str(clp->cl_rpcclient,
							RPC_DISPLAY_ADDR),
				rpc_peeraddr2str(clp->cl_rpcclient,
							RPC_DISPLAY_PROTO),
				clp->cl_rpcclient->cl_auth->au_ops->au_name,
				clp->cl_id_uniquifier);
		setclientid.sc_netid_len = scnprintf(setclientid.sc_netid,
				sizeof(setclientid.sc_netid),
				rpc_peeraddr2str(clp->cl_rpcclient,
							RPC_DISPLAY_NETID));
		setclientid.sc_uaddr_len = scnprintf(setclientid.sc_uaddr,
				sizeof(setclientid.sc_uaddr), "%s.%u.%u",
				clp->cl_ipaddr, port >> 8, port & 255);
		rcu_read_unlock();

		status = rpc_call_sync(clp->cl_rpcclient, &msg, RPC_TASK_TIMEOUT);
		if (status != -NFS4ERR_CLID_INUSE)
			break;
		if (loop != 0) {
			++clp->cl_id_uniquifier;
			break;
		}
		++loop;
		ssleep(clp->cl_lease_time / HZ + 1);
	}
	return status;
}
