xfs_end_io(
	struct work_struct *work)
{
	struct xfs_ioend	*ioend =
		container_of(work, struct xfs_ioend, io_work);
	struct xfs_inode	*ip = XFS_I(ioend->io_inode);
	int			error = ioend->io_bio->bi_error;

	/*
	 * Set an error if the mount has shut down and proceed with end I/O
	 * processing so it can perform whatever cleanups are necessary.
	 */
	if (XFS_FORCED_SHUTDOWN(ip->i_mount))
		error = -EIO;

	/*
	 * For a CoW extent, we need to move the mapping from the CoW fork
	 * to the data fork.  If instead an error happened, just dump the
	 * new blocks.
	 */
	if (ioend->io_type == XFS_IO_COW) {
		if (error)
			goto done;
		if (ioend->io_bio->bi_error) {
			error = xfs_reflink_cancel_cow_range(ip,
					ioend->io_offset, ioend->io_size);
			goto done;
		}
		error = xfs_reflink_end_cow(ip, ioend->io_offset,
				ioend->io_size);
		if (error)
			goto done;
	}

	/*
	 * For unwritten extents we need to issue transactions to convert a
	 * range to normal written extens after the data I/O has finished.
	 * Detecting and handling completion IO errors is done individually
	 * for each case as different cleanup operations need to be performed
	 * on error.
	 */
	if (ioend->io_type == XFS_IO_UNWRITTEN) {
		if (error)
			goto done;
		error = xfs_iomap_write_unwritten(ip, ioend->io_offset,
						  ioend->io_size);
	} else if (ioend->io_append_trans) {
		error = xfs_setfilesize_ioend(ioend, error);
	} else {
		ASSERT(!xfs_ioend_is_append(ioend) ||
		       ioend->io_type == XFS_IO_COW);
	}

done:
	xfs_destroy_ioend(ioend, error);
}
