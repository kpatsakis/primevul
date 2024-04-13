static TEE_Result get_elf_segments(struct user_ta_elf *elf,
				   struct load_seg **segs_ret,
				   size_t *num_segs_ret)
{
	struct elf_load_state *elf_state = elf->elf_state;
	TEE_Result res;
	size_t idx = 0;
	size_t num_segs = 0;
	struct load_seg *segs = NULL;

	/*
	 * Add code segment
	 */
	while (true) {
		vaddr_t va;
		size_t size;
		uint32_t flags;
		uint32_t type;

		res = elf_load_get_next_segment(elf_state, &idx, &va, &size,
						&flags, &type);
		if (res == TEE_ERROR_ITEM_NOT_FOUND)
			break;
		if (res != TEE_SUCCESS)
			return res;

		if (type == PT_LOAD) {
			void *p = realloc(segs, (num_segs + 1) * sizeof(*segs));

			if (!p) {
				free(segs);
				return TEE_ERROR_OUT_OF_MEMORY;
			}
			segs = p;
			segs[num_segs].offs = ROUNDDOWN(va, SMALL_PAGE_SIZE);
			segs[num_segs].oend = ROUNDUP(va + size,
						      SMALL_PAGE_SIZE);
			segs[num_segs].flags = flags;
			num_segs++;
		} else if (type == PT_ARM_EXIDX) {
			elf->exidx_start = va;
			elf->exidx_size = size;
		}
	}

	idx = 1;
	while (idx < num_segs) {
		size_t this_size = segs[idx].oend - segs[idx].offs;
		size_t prev_size = segs[idx - 1].oend - segs[idx - 1].offs;

		if (core_is_buffer_intersect(segs[idx].offs, this_size,
					     segs[idx - 1].offs, prev_size)) {
			/* Merge the segments and their attributes */
			segs[idx - 1].oend = MAX(segs[idx - 1].oend,
						 segs[idx].oend);
			segs[idx - 1].flags |= segs[idx].flags;

			/* Remove this index */
			memcpy(segs + idx, segs + idx + 1,
			       (num_segs - idx - 1) * sizeof(*segs));
			num_segs--;
		} else {
			idx++;
		}
	}

	*segs_ret = segs;
	*num_segs_ret = num_segs;
	return TEE_SUCCESS;
}
