static inline void unregister_as_ext3(void)
{
	unregister_filesystem(&ext3_fs_type);
}
