pvscsi_device_find(PVSCSIState *s, int channel, int target,
                   uint8_t *requested_lun, uint8_t *target_lun)
{
    if (requested_lun[0] || requested_lun[2] || requested_lun[3] ||
        requested_lun[4] || requested_lun[5] || requested_lun[6] ||
        requested_lun[7] || (target > PVSCSI_MAX_DEVS)) {
        return NULL;
    } else {
        *target_lun = requested_lun[1];
        return scsi_device_find(&s->bus, channel, target, *target_lun);
    }
}
