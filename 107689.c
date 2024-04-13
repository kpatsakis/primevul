static void release_ta_memory_by_mobj(struct mobj *mobj)
{
	void *va;

	if (!mobj)
		return;

	va = mobj_get_va(mobj, 0);
	if (!va)
		return;

	memset(va, 0, mobj->size);
	cache_op_inner(DCACHE_AREA_CLEAN, va, mobj->size);
}
