static void free_elfs(struct user_ta_elf_head *elfs)
{
	struct user_ta_elf *elf;
	struct user_ta_elf *next;

	TAILQ_FOREACH_SAFE(elf, elfs, link, next) {
		TAILQ_REMOVE(elfs, elf, link);
		mobj_free(elf->mobj_code);
		free(elf->segs);
		free(elf);
	}
}
