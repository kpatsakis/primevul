static int rtrs_clt_ib_dev_init(struct rtrs_ib_dev *dev)
{
	if (!(dev->ib_dev->attrs.device_cap_flags &
	      IB_DEVICE_MEM_MGT_EXTENSIONS)) {
		pr_err("Memory registrations not supported.\n");
		return -ENOTSUPP;
	}

	return 0;
}