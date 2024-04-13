static int vapic_prepare(VAPICROMState *s)
{
    if (vapic_map_rom_writable(s) < 0) {
        return -1;
    }

    if (patch_hypercalls(s) < 0) {
        return -1;
    }

    vapic_enable_tpr_reporting(true);

    return 0;
}
