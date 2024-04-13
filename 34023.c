gs_pdf14_device_color_mon_set(gx_device *pdev, bool monitoring)
{
    pdf14_device * p14dev = (pdf14_device *)pdev;
    gx_device *targ = p14dev->target;
    cmm_dev_profile_t *dev_profile;
    int code = dev_proc(targ, get_profile)((gx_device*) targ, &dev_profile);

    if (code == 0)
        dev_profile->pageneutralcolor = monitoring;
    return code;
}
