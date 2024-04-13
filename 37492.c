static ssize_t pmcraid_store_log_level(
	struct device *dev,
	struct device_attribute *attr,
	const char *buf,
	size_t count
)
{
	struct Scsi_Host *shost;
	struct pmcraid_instance *pinstance;
	unsigned long val;

	if (strict_strtoul(buf, 10, &val))
		return -EINVAL;
	/* log-level should be from 0 to 2 */
	if (val > 2)
		return -EINVAL;

	shost = class_to_shost(dev);
	pinstance = (struct pmcraid_instance *)shost->hostdata;
	pinstance->current_log_level = val;

	return strlen(buf);
}
