static void nfs4_proc_read_setup(struct nfs_read_data *data, struct rpc_message *msg)
{
	data->timestamp   = jiffies;
	msg->rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_READ];
}
