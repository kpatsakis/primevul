static bool param_mem_from_mobj(struct param_mem *mem, struct mobj *mobj,
				const paddr_t pa, const size_t sz)
{
	paddr_t b;

	if (mobj_get_pa(mobj, 0, 0, &b) != TEE_SUCCESS)
		panic("mobj_get_pa failed");

	if (!core_is_buffer_inside(pa, MAX(sz, 1UL), b, mobj->size))
		return false;

	mem->mobj = mobj;
	mem->offs = pa - b;
	mem->size = sz;
	return true;
}
