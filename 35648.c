int nfs4_init_ds_session(struct nfs_client *clp)
{
	struct nfs4_session *session = clp->cl_session;
	int ret;

	if (!test_and_clear_bit(NFS4_SESSION_INITING, &session->session_state))
		return 0;

	ret = nfs4_client_recover_expired_lease(clp);
	if (!ret)
		/* Test for the DS role */
		if (!is_ds_client(clp))
			ret = -ENODEV;
	if (!ret)
		ret = nfs4_check_client_ready(clp);
	return ret;

}
