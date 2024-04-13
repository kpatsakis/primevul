static int elf_note_info_init(struct elf_note_info *info)
{
	memset(info, 0, sizeof(*info));
	INIT_LIST_HEAD(&info->thread_list);

	/* Allocate space for six ELF notes */
	info->notes = kmalloc(6 * sizeof(struct memelfnote), GFP_KERNEL);
	if (!info->notes)
		return 0;
	info->psinfo = kmalloc(sizeof(*info->psinfo), GFP_KERNEL);
	if (!info->psinfo)
		goto notes_free;
	info->prstatus = kmalloc(sizeof(*info->prstatus), GFP_KERNEL);
	if (!info->prstatus)
		goto psinfo_free;
	info->fpu = kmalloc(sizeof(*info->fpu), GFP_KERNEL);
	if (!info->fpu)
		goto prstatus_free;
#ifdef ELF_CORE_COPY_XFPREGS
	info->xfpu = kmalloc(sizeof(*info->xfpu), GFP_KERNEL);
	if (!info->xfpu)
		goto fpu_free;
#endif
	return 1;
#ifdef ELF_CORE_COPY_XFPREGS
 fpu_free:
	kfree(info->fpu);
#endif
 prstatus_free:
	kfree(info->prstatus);
 psinfo_free:
	kfree(info->psinfo);
 notes_free:
	kfree(info->notes);
	return 0;
}
