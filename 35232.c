off_t size_autodetect(int fhandle) {
	off_t es;
	u64 bytes;
	struct stat stat_buf;
	int error;

#ifdef HAVE_SYS_MOUNT_H
#ifdef HAVE_SYS_IOCTL_H
#ifdef BLKGETSIZE64
	DEBUG("looking for export size with ioctl BLKGETSIZE64\n");
	if (!ioctl(fhandle, BLKGETSIZE64, &bytes) && bytes) {
		return (off_t)bytes;
	}
#endif /* BLKGETSIZE64 */
#endif /* HAVE_SYS_IOCTL_H */
#endif /* HAVE_SYS_MOUNT_H */

	DEBUG("looking for fhandle size with fstat\n");
	stat_buf.st_size = 0;
	error = fstat(fhandle, &stat_buf);
	if (!error) {
		if(stat_buf.st_size > 0)
			return (off_t)stat_buf.st_size;
        } else {
                err("fstat failed: %m");
        }

	DEBUG("looking for fhandle size with lseek SEEK_END\n");
	es = lseek(fhandle, (off_t)0, SEEK_END);
	if (es > ((off_t)0)) {
		return es;
        } else {
                DEBUG2("lseek failed: %d", errno==EBADF?1:(errno==ESPIPE?2:(errno==EINVAL?3:4)));
        }

	err("Could not find size of exported block device: %m");
	return OFFT_MAX;
}
