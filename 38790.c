nfs4_init_uniform_client_string(const struct nfs_client *clp,
				char *buf, size_t len)
{
	char *nodename = clp->cl_rpcclient->cl_nodename;

	if (nfs4_client_id_uniquifier[0] != '\0')
		nodename = nfs4_client_id_uniquifier;
	return scnprintf(buf, len, "Linux NFSv%u.%u %s",
				clp->rpc_ops->version, clp->cl_minorversion,
				nodename);
}
