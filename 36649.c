int nfs_wait_on_sequence(struct nfs_seqid *seqid, struct rpc_task *task)
{
	struct rpc_sequence *sequence = seqid->sequence->sequence;
	int status = 0;

	spin_lock(&sequence->lock);
	if (list_empty(&seqid->list))
		list_add_tail(&seqid->list, &sequence->list);
	if (list_first_entry(&sequence->list, struct nfs_seqid, list) == seqid)
		goto unlock;
	rpc_sleep_on(&sequence->wait, task, NULL);
	status = -EAGAIN;
unlock:
	spin_unlock(&sequence->lock);
	return status;
}
