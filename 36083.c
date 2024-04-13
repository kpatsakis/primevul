static int fill_note_info(struct elfhdr *elf, int phdrs,
			  struct elf_note_info *info,
			  long signr, struct pt_regs *regs)
{
	struct list_head *t;

	if (!elf_note_info_init(info))
		return 0;

	if (signr) {
		struct core_thread *ct;
		struct elf_thread_status *ets;

		for (ct = current->mm->core_state->dumper.next;
						ct; ct = ct->next) {
			ets = kzalloc(sizeof(*ets), GFP_KERNEL);
			if (!ets)
				return 0;

			ets->thread = ct->task;
			list_add(&ets->list, &info->thread_list);
		}

		list_for_each(t, &info->thread_list) {
			int sz;

			ets = list_entry(t, struct elf_thread_status, list);
			sz = elf_dump_thread_status(signr, ets);
			info->thread_status_size += sz;
		}
	}
	/* now collect the dump for the current */
	memset(info->prstatus, 0, sizeof(*info->prstatus));
	fill_prstatus(info->prstatus, current, signr);
	elf_core_copy_regs(&info->prstatus->pr_reg, regs);

	/* Set up header */
	fill_elf_header(elf, phdrs, ELF_ARCH, ELF_CORE_EFLAGS, ELF_OSABI);

	/*
	 * Set up the notes in similar form to SVR4 core dumps made
	 * with info from their /proc.
	 */

	fill_note(info->notes + 0, "CORE", NT_PRSTATUS,
		  sizeof(*info->prstatus), info->prstatus);
	fill_psinfo(info->psinfo, current->group_leader, current->mm);
	fill_note(info->notes + 1, "CORE", NT_PRPSINFO,
		  sizeof(*info->psinfo), info->psinfo);

	info->numnote = 2;

	fill_auxv_note(&info->notes[info->numnote++], current->mm);

	/* Try to dump the FPU. */
	info->prstatus->pr_fpvalid = elf_core_copy_task_fpregs(current, regs,
							       info->fpu);
	if (info->prstatus->pr_fpvalid)
		fill_note(info->notes + info->numnote++,
			  "CORE", NT_PRFPREG, sizeof(*info->fpu), info->fpu);
#ifdef ELF_CORE_COPY_XFPREGS
	if (elf_core_copy_task_xfpregs(current, info->xfpu))
		fill_note(info->notes + info->numnote++,
			  "LINUX", ELF_CORE_XFPREG_TYPE,
			  sizeof(*info->xfpu), info->xfpu);
#endif

	return 1;
}
