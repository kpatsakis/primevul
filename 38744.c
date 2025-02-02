_nfs4_opendata_reclaim_to_nfs4_state(struct nfs4_opendata *data)
{
	struct inode *inode = data->state->inode;
	struct nfs4_state *state = data->state;
	int ret;

	if (!data->rpc_done) {
		ret = data->rpc_status;
		goto err;
	}

	ret = -ESTALE;
	if (!(data->f_attr.valid & NFS_ATTR_FATTR_TYPE) ||
	    !(data->f_attr.valid & NFS_ATTR_FATTR_FILEID) ||
	    !(data->f_attr.valid & NFS_ATTR_FATTR_CHANGE))
		goto err;

	ret = -ENOMEM;
	state = nfs4_get_open_state(inode, data->owner);
	if (state == NULL)
		goto err;

	ret = nfs_refresh_inode(inode, &data->f_attr);
	if (ret)
		goto err;

	if (data->o_res.delegation_type != 0)
		nfs4_opendata_check_deleg(data, state);
	update_open_stateid(state, &data->o_res.stateid, NULL,
			    data->o_arg.fmode);

	return state;
err:
	return ERR_PTR(ret);

}
