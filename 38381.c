static ssize_t qeth_core_driver_group_store(struct device_driver *ddrv,
					    const char *buf, size_t count)
{
	int err;

	err = ccwgroup_create_dev(qeth_core_root_dev,
				  &qeth_core_ccwgroup_driver, 3, buf);

	return err ? err : count;
}
