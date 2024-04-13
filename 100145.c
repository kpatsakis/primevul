void radeon_atom_set_clock_gating(struct radeon_device *rdev, int enable)
{
	DYNAMIC_CLOCK_GATING_PS_ALLOCATION args;
	int index = GetIndexIntoMasterTable(COMMAND, DynamicClockGating);

	args.ucEnable = enable;

	atom_execute_table(rdev->mode_info.atom_context, index, (uint32_t *)&args);
}
