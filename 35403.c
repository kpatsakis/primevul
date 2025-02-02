madvise_vma(struct vm_area_struct *vma, struct vm_area_struct **prev,
		unsigned long start, unsigned long end, int behavior)
{
	switch (behavior) {
	case MADV_REMOVE:
		return madvise_remove(vma, prev, start, end);
	case MADV_WILLNEED:
		return madvise_willneed(vma, prev, start, end);
	case MADV_DONTNEED:
		return madvise_dontneed(vma, prev, start, end);
	default:
		return madvise_behavior(vma, prev, start, end, behavior);
	}
}
