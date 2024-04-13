static void vapic_do_enable_tpr_reporting(void *data)
{
    VAPICEnableTPRReporting *info = data;

    apic_enable_tpr_access_reporting(info->apic, info->enable);
}
