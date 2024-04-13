static void nfs_destroy_inodecache(void)
{
	kmem_cache_destroy(nfs_inode_cachep);
}
