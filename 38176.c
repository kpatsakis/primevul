static ssize_t event_show(struct device *dev,
			  struct device_attribute *attr, char *buf)
{
	struct uio_device *idev = dev_get_drvdata(dev);
	return sprintf(buf, "%u\n", (unsigned int)atomic_read(&idev->event));
}
