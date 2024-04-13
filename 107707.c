static int cmp_param_mem(const void *a0, const void *a1)
{
	const struct param_mem *m1 = a1;
	const struct param_mem *m0 = a0;
	int ret;

	/* Make sure that invalid param_mem are placed last in the array */
	if (!m0->size && !m1->size)
		return 0;
	if (!m0->size)
		return 1;
	if (!m1->size)
		return -1;

	ret = CMP_TRILEAN(mobj_is_secure(m0->mobj), mobj_is_secure(m1->mobj));
	if (ret)
		return ret;

	ret = CMP_TRILEAN((vaddr_t)m0->mobj, (vaddr_t)m1->mobj);
	if (ret)
		return ret;

	ret = CMP_TRILEAN(m0->offs, m1->offs);
	if (ret)
		return ret;

	return CMP_TRILEAN(m0->size, m1->size);
}
