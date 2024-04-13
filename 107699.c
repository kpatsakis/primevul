static void user_ta_dump_state(struct tee_ta_ctx *ctx)
{
	struct user_ta_ctx *utc = to_user_ta_ctx(ctx);
	struct vm_region *r;
	char flags[7] = { '\0', };
	char desc[13];
	size_t n = 0;

	EMSG_RAW(" arch: %s  load address: %#" PRIxVA " ctx-idr: %d",
		 utc->is_32bit ? "arm" : "aarch64", utc->load_addr,
		 utc->vm_info->asid);
	EMSG_RAW(" stack: 0x%" PRIxVA " %zu",
		 utc->stack_addr, utc->mobj_stack->size);
	TAILQ_FOREACH(r, &utc->vm_info->regions, link) {
		paddr_t pa = 0;

		if (r->mobj)
			mobj_get_pa(r->mobj, r->offset, 0, &pa);

		mattr_perm_to_str(flags, sizeof(flags), r->attr);
		describe_region(utc, r->va, r->size, desc, sizeof(desc));
		EMSG_RAW(" region %zu: va %#" PRIxVA " pa %#" PRIxPA
			 " size %#zx flags %s %s",
			 n, r->va, pa, r->size, flags, desc);
		n++;
	}
	show_elfs(utc);
}
