static void show_elfs(struct user_ta_ctx *utc)
{
	struct user_ta_elf *elf;
	size_t __maybe_unused idx = 0;

	TAILQ_FOREACH(elf, &utc->elfs, link)
		EMSG_RAW(" [%zu] %pUl @ %#" PRIxVA, idx++,
			 (void *)&elf->uuid, elf->load_addr);
}
