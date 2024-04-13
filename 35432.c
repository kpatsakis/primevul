static void udf_destroy_inode(struct inode *inode)
{
	call_rcu(&inode->i_rcu, udf_i_callback);
}
