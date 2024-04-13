static void clear_param_map(struct user_ta_ctx *utc)
{
	struct vm_region *next_r;
	struct vm_region *r;

	TAILQ_FOREACH_SAFE(r, &utc->vm_info->regions, link, next_r)
		if (r->attr & TEE_MATTR_EPHEMERAL)
			umap_remove_region(utc->vm_info, r);
}
