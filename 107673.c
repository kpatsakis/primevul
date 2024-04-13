static void describe_region(struct user_ta_ctx *utc, vaddr_t va, size_t size,
			    char *desc, size_t desc_size)
{
	int idx;

	if (!desc_size)
		return;
	idx = elf_idx(utc, va, size);
	if (idx != -1)
		snprintf(desc, desc_size, "[%d]", idx);
	else
		desc[0] = '\0';
	desc[desc_size - 1] = '\0';
}
