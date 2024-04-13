static uint32_t elf_flags_to_mattr(uint32_t flags)
{
	uint32_t mattr = 0;

	if (flags & PF_X)
		mattr |= TEE_MATTR_UX;
	if (flags & PF_W)
		mattr |= TEE_MATTR_UW;
	if (flags & PF_R)
		mattr |= TEE_MATTR_UR;

	return mattr;
}
