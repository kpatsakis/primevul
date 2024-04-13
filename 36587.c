int nfs4_proc_setclientid_confirm(struct nfs_client *clp, struct rpc_cred *cred)
{
	long timeout = 0;
	int err;
	do {
		err = _nfs4_proc_setclientid_confirm(clp, cred);
		switch (err) {
			case 0:
				return err;
			case -NFS4ERR_RESOURCE:
				/* The IBM lawyers misread another document! */
			case -NFS4ERR_DELAY:
				err = nfs4_delay(clp->cl_rpcclient, &timeout);
		}
	} while (err == 0);
	return err;
}
