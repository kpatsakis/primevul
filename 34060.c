pdf14_get_num_spots(gx_device * dev)
{
    cmm_dev_profile_t *dev_profile;
    cmm_profile_t *icc_profile;
    gsicc_rendering_param_t render_cond;

    dev_proc(dev, get_profile)(dev, &dev_profile);
    gsicc_extract_profile(GS_UNKNOWN_TAG, dev_profile, &icc_profile,
        &render_cond);
    return dev->color_info.num_components - icc_profile->num_comps;
}
