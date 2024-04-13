static int nfs4_handle_exception(const struct nfs_server *server, int errorcode, struct nfs4_exception *exception)
{
	struct nfs_client *clp = server->nfs_client;
	struct nfs4_state *state = exception->state;
	int ret = errorcode;

	exception->retry = 0;
	switch(errorcode) {
		case 0:
			return 0;
		case -NFS4ERR_ADMIN_REVOKED:
		case -NFS4ERR_BAD_STATEID:
		case -NFS4ERR_OPENMODE:
			if (state == NULL)
				break;
			nfs4_state_mark_reclaim_nograce(clp, state);
		case -NFS4ERR_STALE_CLIENTID:
		case -NFS4ERR_STALE_STATEID:
		case -NFS4ERR_EXPIRED:
			nfs4_schedule_state_recovery(clp);
			ret = nfs4_wait_clnt_recover(clp);
			if (ret == 0)
				exception->retry = 1;
			break;
		case -NFS4ERR_FILE_OPEN:
		case -NFS4ERR_GRACE:
		case -NFS4ERR_DELAY:
			ret = nfs4_delay(server->client, &exception->timeout);
			if (ret != 0)
				break;
		case -NFS4ERR_OLD_STATEID:
			exception->retry = 1;
	}
	/* We failed to handle the error */
	return nfs4_map_errors(ret);
}
