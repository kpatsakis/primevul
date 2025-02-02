static void show_map_vma(struct seq_file *m, struct vm_area_struct *vma)
{
	struct mm_struct *mm = vma->vm_mm;
	struct file *file = vma->vm_file;
	int flags = vma->vm_flags;
	unsigned long ino = 0;
	unsigned long long pgoff = 0;
	unsigned long start;
	dev_t dev = 0;
	int len;

	if (file) {
		struct inode *inode = vma->vm_file->f_path.dentry->d_inode;
		dev = inode->i_sb->s_dev;
		ino = inode->i_ino;
		pgoff = ((loff_t)vma->vm_pgoff) << PAGE_SHIFT;
	}

	/* We don't show the stack guard page in /proc/maps */
	start = vma->vm_start;
	if (vma->vm_flags & VM_GROWSDOWN)
		if (!vma_stack_continue(vma->vm_prev, vma->vm_start))
			start += PAGE_SIZE;

	seq_printf(m, "%08lx-%08lx %c%c%c%c %08llx %02x:%02x %lu %n",
			start,
			vma->vm_end,
			flags & VM_READ ? 'r' : '-',
			flags & VM_WRITE ? 'w' : '-',
			flags & VM_EXEC ? 'x' : '-',
			flags & VM_MAYSHARE ? 's' : 'p',
			pgoff,
			MAJOR(dev), MINOR(dev), ino, &len);

	/*
	 * Print the dentry name for named mappings, and a
	 * special [heap] marker for the heap:
	 */
	if (file) {
		pad_len_spaces(m, len);
		seq_path(m, &file->f_path, "\n");
	} else {
		const char *name = arch_vma_name(vma);
		if (!name) {
			if (mm) {
				if (vma->vm_start <= mm->brk &&
						vma->vm_end >= mm->start_brk) {
					name = "[heap]";
				} else if (vma->vm_start <= mm->start_stack &&
					   vma->vm_end >= mm->start_stack) {
					name = "[stack]";
				}
			} else {
				name = "[vdso]";
			}
		}
		if (name) {
			pad_len_spaces(m, len);
			seq_puts(m, name);
		}
	}
	seq_putc(m, '\n');
}
