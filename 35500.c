static int reverse_path_check_proc(void *priv, void *cookie, int call_nests)
{
	int error = 0;
	struct file *file = priv;
	struct file *child_file;
	struct epitem *epi;

	list_for_each_entry(epi, &file->f_ep_links, fllink) {
		child_file = epi->ep->file;
		if (is_file_epoll(child_file)) {
			if (list_empty(&child_file->f_ep_links)) {
				if (path_count_inc(call_nests)) {
					error = -1;
					break;
				}
			} else {
				error = ep_call_nested(&poll_loop_ncalls,
							EP_MAX_NESTS,
							reverse_path_check_proc,
							child_file, child_file,
							current);
			}
			if (error != 0)
				break;
		} else {
			printk(KERN_ERR "reverse_path_check_proc: "
				"file is not an ep!\n");
		}
	}
	return error;
}
