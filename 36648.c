static void nfs_increment_seqid(int status, struct nfs_seqid *seqid)
{
	BUG_ON(list_first_entry(&seqid->sequence->sequence->list, struct nfs_seqid, list) != seqid);
	switch (status) {
		case 0:
			break;
		case -NFS4ERR_BAD_SEQID:
			if (seqid->sequence->flags & NFS_SEQID_CONFIRMED)
				return;
			printk(KERN_WARNING "NFS: v4 server returned a bad"
					" sequence-id error on an"
					" unconfirmed sequence %p!\n",
					seqid->sequence);
		case -NFS4ERR_STALE_CLIENTID:
		case -NFS4ERR_STALE_STATEID:
		case -NFS4ERR_BAD_STATEID:
		case -NFS4ERR_BADXDR:
		case -NFS4ERR_RESOURCE:
		case -NFS4ERR_NOFILEHANDLE:
			/* Non-seqid mutating errors */
			return;
	};
	/*
	 * Note: no locking needed as we are guaranteed to be first
	 * on the sequence list
	 */
	seqid->sequence->counter++;
}
