static int uio_mmap_logical(struct vm_area_struct *vma)
{
	vma->vm_flags |= VM_DONTEXPAND | VM_DONTDUMP;
	vma->vm_ops = &uio_logical_vm_ops;
	return 0;
}
