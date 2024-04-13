static void sock_destroy_inode(struct inode *inode)
{
	struct socket_alloc *ei;
	struct socket_wq *wq;

	ei = container_of(inode, struct socket_alloc, vfs_inode);
	wq = rcu_dereference_protected(ei->socket.wq, 1);
	kfree_rcu(wq, rcu);
	kmem_cache_free(sock_inode_cachep, ei);
}
