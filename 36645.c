void nfs_free_seqid(struct nfs_seqid *seqid)
{
	if (!list_empty(&seqid->list)) {
		struct rpc_sequence *sequence = seqid->sequence->sequence;

		spin_lock(&sequence->lock);
		list_del(&seqid->list);
		spin_unlock(&sequence->lock);
		rpc_wake_up(&sequence->wait);
	}
	kfree(seqid);
}
