void tee_mmu_rem_rwmem(struct user_ta_ctx *utc, struct mobj *mobj, vaddr_t va)
{
	struct vm_region *reg;

	TAILQ_FOREACH(reg, &utc->vm_info->regions, link) {
		if (reg->mobj == mobj && reg->va == va) {
			free_pgt(utc, reg->va, reg->size);
			umap_remove_region(utc->vm_info, reg);
			return;
		}
	}
}
