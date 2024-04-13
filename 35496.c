static inline int is_file_epoll(struct file *f)
{
	return f->f_op == &eventpoll_fops;
}
