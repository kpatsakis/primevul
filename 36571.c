int nfs4_open_delegation_recall(struct nfs_open_context *ctx, struct nfs4_state *state, const nfs4_stateid *stateid)
{
	struct nfs4_exception exception = { };
	struct nfs_server *server = NFS_SERVER(state->inode);
	int err;
	do {
		err = _nfs4_open_delegation_recall(ctx, state, stateid);
		switch (err) {
			case 0:
				return err;
			case -NFS4ERR_STALE_CLIENTID:
			case -NFS4ERR_STALE_STATEID:
			case -NFS4ERR_EXPIRED:
				/* Don't recall a delegation if it was lost */
				nfs4_schedule_state_recovery(server->nfs_client);
				return err;
		}
		err = nfs4_handle_exception(server, err, &exception);
	} while (exception.retry);
	return err;
}
