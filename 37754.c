asmlinkage long sys_oabi_epoll_ctl(int epfd, int op, int fd,
				   struct oabi_epoll_event __user *event)
{
	struct oabi_epoll_event user;
	struct epoll_event kernel;
	mm_segment_t fs;
	long ret;

	if (op == EPOLL_CTL_DEL)
		return sys_epoll_ctl(epfd, op, fd, NULL);
	if (copy_from_user(&user, event, sizeof(user)))
		return -EFAULT;
	kernel.events = user.events;
	kernel.data   = user.data;
	fs = get_fs();
	set_fs(KERNEL_DS);
	ret = sys_epoll_ctl(epfd, op, fd, &kernel);
	set_fs(fs);
	return ret;
}
