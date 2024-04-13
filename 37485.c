static ssize_t pmcraid_show_drv_version(
	struct device *dev,
	struct device_attribute *attr,
	char *buf
)
{
	return snprintf(buf, PAGE_SIZE, "version: %s\n",
			PMCRAID_DRIVER_VERSION);
}
