static ssize_t aac_store_reset_adapter(struct device *device,
				       struct device_attribute *attr,
				       const char *buf, size_t count)
{
	int retval = -EACCES;

	if (!capable(CAP_SYS_ADMIN))
		return retval;
	retval = aac_reset_adapter((struct aac_dev*)class_to_shost(device)->hostdata, buf[0] == '!');
	if (retval >= 0)
		retval = count;
	return retval;
}
