nfs4_opendata_to_nfs4_state(struct nfs4_opendata *data)
{
	if (data->o_arg.claim == NFS4_OPEN_CLAIM_PREVIOUS)
		return _nfs4_opendata_reclaim_to_nfs4_state(data);
	return _nfs4_opendata_to_nfs4_state(data);
}
