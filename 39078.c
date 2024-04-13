static int oz_cdev_open(struct inode *inode, struct file *filp)
{
	struct oz_cdev *dev = container_of(inode->i_cdev, struct oz_cdev, cdev);

	oz_dbg(ON, "major = %d minor = %d\n", imajor(inode), iminor(inode));

	filp->private_data = dev;
	return 0;
}
