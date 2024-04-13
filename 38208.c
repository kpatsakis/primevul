static ssize_t version_show(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	struct uio_device *idev = dev_get_drvdata(dev);
	return sprintf(buf, "%s\n", idev->info->version);
}
