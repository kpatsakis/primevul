xfs_end_io_direct_write(
	struct kiocb		*iocb,
	loff_t			offset,
	ssize_t			size,
	void			*private)
{
	struct inode		*inode = file_inode(iocb->ki_filp);
	struct xfs_inode	*ip = XFS_I(inode);
	uintptr_t		flags = (uintptr_t)private;
	int			error = 0;

	trace_xfs_end_io_direct_write(ip, offset, size);

	if (XFS_FORCED_SHUTDOWN(ip->i_mount))
		return -EIO;

	if (size <= 0)
		return size;

	/*
	 * The flags tell us whether we are doing unwritten extent conversions
	 * or an append transaction that updates the on-disk file size. These
	 * cases are the only cases where we should *potentially* be needing
	 * to update the VFS inode size.
	 */
	if (flags == 0) {
		ASSERT(offset + size <= i_size_read(inode));
		return 0;
	}

	/*
	 * We need to update the in-core inode size here so that we don't end up
	 * with the on-disk inode size being outside the in-core inode size. We
	 * have no other method of updating EOF for AIO, so always do it here
	 * if necessary.
	 *
	 * We need to lock the test/set EOF update as we can be racing with
	 * other IO completions here to update the EOF. Failing to serialise
	 * here can result in EOF moving backwards and Bad Things Happen when
	 * that occurs.
	 */
	spin_lock(&ip->i_flags_lock);
	if (offset + size > i_size_read(inode))
		i_size_write(inode, offset + size);
	spin_unlock(&ip->i_flags_lock);

	if (flags & XFS_DIO_FLAG_COW)
		error = xfs_reflink_end_cow(ip, offset, size);
	if (flags & XFS_DIO_FLAG_UNWRITTEN) {
		trace_xfs_end_io_direct_write_unwritten(ip, offset, size);

		error = xfs_iomap_write_unwritten(ip, offset, size);
	}
	if (flags & XFS_DIO_FLAG_APPEND) {
		trace_xfs_end_io_direct_write_append(ip, offset, size);

		error = xfs_setfilesize(ip, offset, size);
	}

	return error;
}
