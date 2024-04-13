static int megasas_ctrl_get_info(MegasasState *s, MegasasCmd *cmd)
{
    PCIDevice *pci_dev = PCI_DEVICE(s);
    PCIDeviceClass *pci_class = PCI_DEVICE_GET_CLASS(pci_dev);
    MegasasBaseClass *base_class = MEGASAS_DEVICE_GET_CLASS(s);
    struct mfi_ctrl_info info;
    size_t dcmd_size = sizeof(info);
    BusChild *kid;
    int num_pd_disks = 0;

    memset(&info, 0x0, dcmd_size);
    if (cmd->iov_size < dcmd_size) {
        trace_megasas_dcmd_invalid_xfer_len(cmd->index, cmd->iov_size,
                                            dcmd_size);
        return MFI_STAT_INVALID_PARAMETER;
    }

    info.pci.vendor = cpu_to_le16(pci_class->vendor_id);
    info.pci.device = cpu_to_le16(pci_class->device_id);
    info.pci.subvendor = cpu_to_le16(pci_class->subsystem_vendor_id);
    info.pci.subdevice = cpu_to_le16(pci_class->subsystem_id);

    /*
     * For some reason the firmware supports
     * only up to 8 device ports.
     * Despite supporting a far larger number
     * of devices for the physical devices.
     * So just display the first 8 devices
     * in the device port list, independent
     * of how many logical devices are actually
     * present.
     */
    info.host.type = MFI_INFO_HOST_PCIE;
    info.device.type = MFI_INFO_DEV_SAS3G;
    info.device.port_count = 8;
    QTAILQ_FOREACH(kid, &s->bus.qbus.children, sibling) {
        SCSIDevice *sdev = SCSI_DEVICE(kid->child);
        uint16_t pd_id;

        if (num_pd_disks < 8) {
            pd_id = ((sdev->id & 0xFF) << 8) | (sdev->lun & 0xFF);
            info.device.port_addr[num_pd_disks] =
                cpu_to_le64(megasas_get_sata_addr(pd_id));
        }
        num_pd_disks++;
    }

    memcpy(info.product_name, base_class->product_name, 24);
    snprintf(info.serial_number, 32, "%s", s->hba_serial);
    snprintf(info.package_version, 0x60, "%s-QEMU", qemu_hw_version());
    memcpy(info.image_component[0].name, "APP", 3);
    snprintf(info.image_component[0].version, 10, "%s-QEMU",
             base_class->product_version);
    memcpy(info.image_component[0].build_date, "Apr  1 2014", 11);
    memcpy(info.image_component[0].build_time, "12:34:56", 8);
    info.image_component_count = 1;
    if (pci_dev->has_rom) {
        uint8_t biosver[32];
        uint8_t *ptr;
 
         ptr = memory_region_get_ram_ptr(&pci_dev->rom);
         memcpy(biosver, ptr + 0x41, 31);
        biosver[31] = 0;
         memcpy(info.image_component[1].name, "BIOS", 4);
         memcpy(info.image_component[1].version, biosver,
                strlen((const char *)biosver));
    }
    info.current_fw_time = cpu_to_le32(megasas_fw_time());
    info.max_arms = 32;
    info.max_spans = 8;
    info.max_arrays = MEGASAS_MAX_ARRAYS;
    info.max_lds = MFI_MAX_LD;
    info.max_cmds = cpu_to_le16(s->fw_cmds);
    info.max_sg_elements = cpu_to_le16(s->fw_sge);
    info.max_request_size = cpu_to_le32(MEGASAS_MAX_SECTORS);
    if (!megasas_is_jbod(s))
        info.lds_present = cpu_to_le16(num_pd_disks);
    info.pd_present = cpu_to_le16(num_pd_disks);
    info.pd_disks_present = cpu_to_le16(num_pd_disks);
    info.hw_present = cpu_to_le32(MFI_INFO_HW_NVRAM |
                                   MFI_INFO_HW_MEM |
                                   MFI_INFO_HW_FLASH);
    info.memory_size = cpu_to_le16(512);
    info.nvram_size = cpu_to_le16(32);
    info.flash_size = cpu_to_le16(16);
    info.raid_levels = cpu_to_le32(MFI_INFO_RAID_0);
    info.adapter_ops = cpu_to_le32(MFI_INFO_AOPS_RBLD_RATE |
                                    MFI_INFO_AOPS_SELF_DIAGNOSTIC |
                                    MFI_INFO_AOPS_MIXED_ARRAY);
    info.ld_ops = cpu_to_le32(MFI_INFO_LDOPS_DISK_CACHE_POLICY |
                               MFI_INFO_LDOPS_ACCESS_POLICY |
                               MFI_INFO_LDOPS_IO_POLICY |
                               MFI_INFO_LDOPS_WRITE_POLICY |
                               MFI_INFO_LDOPS_READ_POLICY);
    info.max_strips_per_io = cpu_to_le16(s->fw_sge);
    info.stripe_sz_ops.min = 3;
    info.stripe_sz_ops.max = ctz32(MEGASAS_MAX_SECTORS + 1);
    info.properties.pred_fail_poll_interval = cpu_to_le16(300);
    info.properties.intr_throttle_cnt = cpu_to_le16(16);
    info.properties.intr_throttle_timeout = cpu_to_le16(50);
    info.properties.rebuild_rate = 30;
    info.properties.patrol_read_rate = 30;
    info.properties.bgi_rate = 30;
    info.properties.cc_rate = 30;
    info.properties.recon_rate = 30;
    info.properties.cache_flush_interval = 4;
    info.properties.spinup_drv_cnt = 2;
    info.properties.spinup_delay = 6;
    info.properties.ecc_bucket_size = 15;
    info.properties.ecc_bucket_leak_rate = cpu_to_le16(1440);
    info.properties.expose_encl_devices = 1;
    info.properties.OnOffProperties = cpu_to_le32(MFI_CTRL_PROP_EnableJBOD);
    info.pd_ops = cpu_to_le32(MFI_INFO_PDOPS_FORCE_ONLINE |
                               MFI_INFO_PDOPS_FORCE_OFFLINE);
    info.pd_mix_support = cpu_to_le32(MFI_INFO_PDMIX_SAS |
                                       MFI_INFO_PDMIX_SATA |
                                       MFI_INFO_PDMIX_LD);

    cmd->iov_size -= dma_buf_read((uint8_t *)&info, dcmd_size, &cmd->qsg);
    return MFI_STAT_OK;
}
