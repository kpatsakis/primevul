find_extend_vma(struct mm_struct *mm, unsigned long addr)
{
	struct vm_area_struct *vma;
	unsigned long start;

	addr &= PAGE_MASK;
	vma = find_vma(mm, addr);
	if (!vma)
		return NULL;
	if (vma->vm_start <= addr)
 		return vma;
 	if (!(vma->vm_flags & VM_GROWSDOWN))
 		return NULL;
	/* don't alter vm_start if the coredump is running */
	if (!mmget_still_valid(mm))
		return NULL;
 	start = vma->vm_start;
 	if (expand_stack(vma, addr))
 		return NULL;
	if (vma->vm_flags & VM_LOCKED)
		populate_vma_page_range(vma, addr, start, NULL);
	return vma;
}
