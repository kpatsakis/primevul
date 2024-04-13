static USBDevice *usb_net_init(USBBus *bus, const char *cmdline)
{
    Error *local_err = NULL;
    USBDevice *dev;
    QemuOpts *opts;
    int idx;

    opts = qemu_opts_parse_noisily(qemu_find_opts("net"), cmdline, false);
    if (!opts) {
        return NULL;
    }
    qemu_opt_set(opts, "type", "nic", &error_abort);
    qemu_opt_set(opts, "model", "usb", &error_abort);

    idx = net_client_init(opts, 0, &local_err);
    if (local_err) {
        error_report_err(local_err);
        return NULL;
    }

    dev = usb_create(bus, "usb-net");
    qdev_set_nic_properties(&dev->qdev, &nd_table[idx]);
    return dev;
}
