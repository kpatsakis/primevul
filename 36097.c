static int write_note_info(struct elf_note_info *info,
			   struct file *file, loff_t *foffset)
{
	bool first = 1;
	struct elf_thread_core_info *t = info->thread;

	do {
		int i;

		if (!writenote(&t->notes[0], file, foffset))
			return 0;

		if (first && !writenote(&info->psinfo, file, foffset))
			return 0;
		if (first && !writenote(&info->auxv, file, foffset))
			return 0;

		for (i = 1; i < info->thread_notes; ++i)
			if (t->notes[i].data &&
			    !writenote(&t->notes[i], file, foffset))
				return 0;

		first = 0;
		t = t->next;
	} while (t);

	return 1;
}
