bool tee_mmu_is_vbuf_inside_ta_private(const struct user_ta_ctx *utc,
				  const void *va, size_t size)
{
	struct vm_region *r;

	TAILQ_FOREACH(r, &utc->vm_info->regions, link) {
		if (r->attr & (TEE_MATTR_EPHEMERAL | TEE_MATTR_PERMANENT))
			continue;
		if (core_is_buffer_inside(va, size, r->va, r->size))
			return true;
	}

	return false;
}
