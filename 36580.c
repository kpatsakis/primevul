static int nfs4_proc_lookupfh(struct nfs_server *server, struct nfs_fh *dirfh,
			      struct qstr *name, struct nfs_fh *fhandle,
			      struct nfs_fattr *fattr)
{
	struct nfs4_exception exception = { };
	int err;
	do {
		err = _nfs4_proc_lookupfh(server, dirfh, name, fhandle, fattr);
		/* FIXME: !!!! */
		if (err == -NFS4ERR_MOVED) {
			err = -EREMOTE;
			break;
		}
		err = nfs4_handle_exception(server, err, &exception);
	} while (exception.retry);
	return err;
}
