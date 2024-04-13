static unsigned mounts_poll(struct file *file, poll_table *wait)
{
	struct proc_mounts *p = file->private_data;
	unsigned res = POLLIN | POLLRDNORM;

	poll_wait(file, &p->ns->poll, wait);
	if (mnt_had_events(p))
		res |= POLLERR | POLLPRI;

	return res;
}
