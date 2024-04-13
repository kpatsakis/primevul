static inline void unregister_as_ext2(void)
{
	unregister_filesystem(&ext2_fs_type);
}
