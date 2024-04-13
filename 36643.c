struct nfs_seqid *nfs_alloc_seqid(struct nfs_seqid_counter *counter)
{
	struct nfs_seqid *new;

	new = kmalloc(sizeof(*new), GFP_KERNEL);
	if (new != NULL) {
		new->sequence = counter;
		INIT_LIST_HEAD(&new->list);
	}
	return new;
}
