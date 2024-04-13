static void vapic_reset(DeviceState *dev)
{
    VAPICROMState *s = VAPIC(dev);

    s->state = VAPIC_INACTIVE;
    s->rom_state_paddr = 0;
    vapic_enable_tpr_reporting(false);
}
