static void free_elf_states(struct user_ta_ctx *utc)
{
	struct user_ta_elf *elf;

	TAILQ_FOREACH(elf, &utc->elfs, link)
			elf_load_final(elf->elf_state);
}
