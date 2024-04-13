static int compat_watcher_to_user(struct ebt_entry_watcher *w,
				  void __user **dstptr,
				  unsigned int *size)
{
	return compat_target_to_user((struct ebt_entry_target *)w,
							dstptr, size);
}
