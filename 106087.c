static void vmxnet3_update_mcast_filters(VMXNET3State *s)
{
    uint16_t list_bytes =
        VMXNET3_READ_DRV_SHARED16(s->drv_shmem,
                                  devRead.rxFilterConf.mfTableLen);

    s->mcast_list_len = list_bytes / sizeof(s->mcast_list[0]);

    s->mcast_list = g_realloc(s->mcast_list, list_bytes);
    if (NULL == s->mcast_list) {
        if (0 == s->mcast_list_len) {
            VMW_CFPRN("Current multicast list is empty");
        } else {
            VMW_ERPRN("Failed to allocate multicast list of %d elements",
                      s->mcast_list_len);
        }
        s->mcast_list_len = 0;
    } else {
        int i;
        hwaddr mcast_list_pa =
            VMXNET3_READ_DRV_SHARED64(s->drv_shmem,
                                      devRead.rxFilterConf.mfTablePA);

        cpu_physical_memory_read(mcast_list_pa, s->mcast_list, list_bytes);
        VMW_CFPRN("Current multicast list len is %d:", s->mcast_list_len);
        for (i = 0; i < s->mcast_list_len; i++) {
            VMW_CFPRN("\t" VMXNET_MF, VMXNET_MA(s->mcast_list[i].a));
        }
    }
}
