static int prctl_set_mm(int opt, unsigned long addr,
			unsigned long arg4, unsigned long arg5)
{
	unsigned long rlim = rlimit(RLIMIT_DATA);
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;
	int error;

	if (arg5 || (arg4 && opt != PR_SET_MM_AUXV))
		return -EINVAL;

	if (!capable(CAP_SYS_RESOURCE))
		return -EPERM;

	if (opt == PR_SET_MM_EXE_FILE)
		return prctl_set_mm_exe_file(mm, (unsigned int)addr);

	if (addr >= TASK_SIZE || addr < mmap_min_addr)
		return -EINVAL;

	error = -EINVAL;

	down_read(&mm->mmap_sem);
	vma = find_vma(mm, addr);

	switch (opt) {
	case PR_SET_MM_START_CODE:
		mm->start_code = addr;
		break;
	case PR_SET_MM_END_CODE:
		mm->end_code = addr;
		break;
	case PR_SET_MM_START_DATA:
		mm->start_data = addr;
		break;
	case PR_SET_MM_END_DATA:
		mm->end_data = addr;
		break;

	case PR_SET_MM_START_BRK:
		if (addr <= mm->end_data)
			goto out;

		if (rlim < RLIM_INFINITY &&
		    (mm->brk - addr) +
		    (mm->end_data - mm->start_data) > rlim)
			goto out;

		mm->start_brk = addr;
		break;

	case PR_SET_MM_BRK:
		if (addr <= mm->end_data)
			goto out;

		if (rlim < RLIM_INFINITY &&
		    (addr - mm->start_brk) +
		    (mm->end_data - mm->start_data) > rlim)
			goto out;

		mm->brk = addr;
		break;

	/*
	 * If command line arguments and environment
	 * are placed somewhere else on stack, we can
	 * set them up here, ARG_START/END to setup
	 * command line argumets and ENV_START/END
	 * for environment.
	 */
	case PR_SET_MM_START_STACK:
	case PR_SET_MM_ARG_START:
	case PR_SET_MM_ARG_END:
	case PR_SET_MM_ENV_START:
	case PR_SET_MM_ENV_END:
		if (!vma) {
			error = -EFAULT;
			goto out;
		}
		if (opt == PR_SET_MM_START_STACK)
			mm->start_stack = addr;
		else if (opt == PR_SET_MM_ARG_START)
			mm->arg_start = addr;
		else if (opt == PR_SET_MM_ARG_END)
			mm->arg_end = addr;
		else if (opt == PR_SET_MM_ENV_START)
			mm->env_start = addr;
		else if (opt == PR_SET_MM_ENV_END)
			mm->env_end = addr;
		break;

	/*
	 * This doesn't move auxiliary vector itself
	 * since it's pinned to mm_struct, but allow
	 * to fill vector with new values. It's up
	 * to a caller to provide sane values here
	 * otherwise user space tools which use this
	 * vector might be unhappy.
	 */
	case PR_SET_MM_AUXV: {
		unsigned long user_auxv[AT_VECTOR_SIZE];

		if (arg4 > sizeof(user_auxv))
			goto out;
		up_read(&mm->mmap_sem);

		if (copy_from_user(user_auxv, (const void __user *)addr, arg4))
			return -EFAULT;

		/* Make sure the last entry is always AT_NULL */
		user_auxv[AT_VECTOR_SIZE - 2] = 0;
		user_auxv[AT_VECTOR_SIZE - 1] = 0;

		BUILD_BUG_ON(sizeof(user_auxv) != sizeof(mm->saved_auxv));

		task_lock(current);
		memcpy(mm->saved_auxv, user_auxv, arg4);
		task_unlock(current);

		return 0;
	}
	default:
		goto out;
	}

	error = 0;
out:
	up_read(&mm->mmap_sem);
	return error;
}
