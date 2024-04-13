static unsigned short pmcraid_get_minor(void)
{
	int minor;

	minor = find_first_zero_bit(pmcraid_minor, sizeof(pmcraid_minor));
	__set_bit(minor, pmcraid_minor);
	return minor;
}
