static int _nfs4_proc_lookup(struct inode *dir, const struct qstr *name,
		struct nfs_fh *fhandle, struct nfs_fattr *fattr)
{
	int status;
	
	dprintk("NFS call  lookup %s\n", name->name);
	status = _nfs4_proc_lookupfh(NFS_SERVER(dir), NFS_FH(dir), name, fhandle, fattr);
	if (status == -NFS4ERR_MOVED)
		status = nfs4_get_referral(dir, name, fattr, fhandle);
	dprintk("NFS reply lookup: %d\n", status);
	return status;
}
