static ssize_t clear_refs_write(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	struct task_struct *task;
	char buffer[PROC_NUMBUF];
	struct mm_struct *mm;
	struct vm_area_struct *vma;
	long type;

	memset(buffer, 0, sizeof(buffer));
	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;
	if (copy_from_user(buffer, buf, count))
		return -EFAULT;
	if (strict_strtol(strstrip(buffer), 10, &type))
		return -EINVAL;
	if (type < CLEAR_REFS_ALL || type > CLEAR_REFS_MAPPED)
		return -EINVAL;
	task = get_proc_task(file->f_path.dentry->d_inode);
	if (!task)
		return -ESRCH;
	mm = get_task_mm(task);
	if (mm) {
		struct mm_walk clear_refs_walk = {
			.pmd_entry = clear_refs_pte_range,
			.mm = mm,
		};
		down_read(&mm->mmap_sem);
		for (vma = mm->mmap; vma; vma = vma->vm_next) {
			clear_refs_walk.private = vma;
			if (is_vm_hugetlb_page(vma))
				continue;
			/*
			 * Writing 1 to /proc/pid/clear_refs affects all pages.
			 *
			 * Writing 2 to /proc/pid/clear_refs only affects
			 * Anonymous pages.
			 *
			 * Writing 3 to /proc/pid/clear_refs only affects file
			 * mapped pages.
			 */
			if (type == CLEAR_REFS_ANON && vma->vm_file)
				continue;
			if (type == CLEAR_REFS_MAPPED && !vma->vm_file)
				continue;
			walk_page_range(vma->vm_start, vma->vm_end,
					&clear_refs_walk);
		}
		flush_tlb_mm(mm);
		up_read(&mm->mmap_sem);
		mmput(mm);
	}
	put_task_struct(task);

	return count;
}
