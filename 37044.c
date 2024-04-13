static int mmtimer_ioctl(struct inode *inode, struct file *file,
			 unsigned int cmd, unsigned long arg)
{
	int ret = 0;

	switch (cmd) {
	case MMTIMER_GETOFFSET:	/* offset of the counter */
		/*
		 * SN RTC registers are on their own 64k page
		 */
		if(PAGE_SIZE <= (1 << 16))
			ret = (((long)RTC_COUNTER_ADDR) & (PAGE_SIZE-1)) / 8;
		else
			ret = -ENOSYS;
		break;

	case MMTIMER_GETRES: /* resolution of the clock in 10^-15 s */
		if(copy_to_user((unsigned long __user *)arg,
				&mmtimer_femtoperiod, sizeof(unsigned long)))
			return -EFAULT;
		break;

	case MMTIMER_GETFREQ: /* frequency in Hz */
		if(copy_to_user((unsigned long __user *)arg,
				&sn_rtc_cycles_per_second,
				sizeof(unsigned long)))
			return -EFAULT;
		ret = 0;
		break;

	case MMTIMER_GETBITS: /* number of bits in the clock */
		ret = RTC_BITS;
		break;

	case MMTIMER_MMAPAVAIL: /* can we mmap the clock into userspace? */
		ret = (PAGE_SIZE <= (1 << 16)) ? 1 : 0;
		break;

	case MMTIMER_GETCOUNTER:
		if(copy_to_user((unsigned long __user *)arg,
				RTC_COUNTER_ADDR, sizeof(unsigned long)))
			return -EFAULT;
		break;
	default:
		ret = -ENOSYS;
		break;
	}

	return ret;
}
