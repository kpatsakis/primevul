static int mmtimer_mmap(struct file *file, struct vm_area_struct *vma)
{
	unsigned long mmtimer_addr;

	if (vma->vm_end - vma->vm_start != PAGE_SIZE)
		return -EINVAL;

	if (vma->vm_flags & VM_WRITE)
		return -EPERM;

	if (PAGE_SIZE > (1 << 16))
		return -ENOSYS;

	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	mmtimer_addr = __pa(RTC_COUNTER_ADDR);
	mmtimer_addr &= ~(PAGE_SIZE - 1);
	mmtimer_addr &= 0xfffffffffffffffUL;

	if (remap_pfn_range(vma, vma->vm_start, mmtimer_addr >> PAGE_SHIFT,
					PAGE_SIZE, vma->vm_page_prot)) {
		printk(KERN_ERR "remap_pfn_range failed in mmtimer.c\n");
		return -EAGAIN;
	}

	return 0;
}
