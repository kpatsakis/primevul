nfs4_init_nonuniform_client_string(const struct nfs_client *clp,
				   char *buf, size_t len)
{
	unsigned int result;

	rcu_read_lock();
	result = scnprintf(buf, len, "Linux NFSv4.0 %s/%s %s",
				clp->cl_ipaddr,
				rpc_peeraddr2str(clp->cl_rpcclient,
							RPC_DISPLAY_ADDR),
				rpc_peeraddr2str(clp->cl_rpcclient,
							RPC_DISPLAY_PROTO));
	rcu_read_unlock();
	return result;
}
