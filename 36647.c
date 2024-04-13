void nfs_increment_open_seqid(int status, struct nfs_seqid *seqid)
{
	if (status == -NFS4ERR_BAD_SEQID) {
		struct nfs4_state_owner *sp = container_of(seqid->sequence,
				struct nfs4_state_owner, so_seqid);
		nfs4_drop_state_owner(sp);
	}
	nfs_increment_seqid(status, seqid);
}
