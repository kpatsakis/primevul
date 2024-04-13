static struct user_ta_elf *ta_elf(const TEE_UUID *uuid,
				  struct user_ta_ctx *utc)
{
	struct user_ta_elf *elf;

	elf = find_ta_elf(uuid, utc);
	if (elf)
		goto out;
	elf = calloc(1, sizeof(*elf));
	if (!elf)
		goto out;
	elf->uuid = *uuid;

	TAILQ_INSERT_TAIL(&utc->elfs, elf, link);
out:
	return elf;
}
