static int vapic_post_load(void *opaque, int version_id)
{
    VAPICROMState *s = opaque;

    /*
     * The old implementation of qemu-kvm did not provide the state
     * VAPIC_STANDBY. Reconstruct it.
     */
    if (s->state == VAPIC_INACTIVE && s->rom_state_paddr != 0) {
        s->state = VAPIC_STANDBY;
    }

    if (s->state != VAPIC_INACTIVE) {
        if (vapic_prepare(s) < 0) {
            return -1;
        }
    }

    if (!s->vmsentry) {
        s->vmsentry =
            qemu_add_vm_change_state_handler(kvmvapic_vm_state_change, s);
    }
    return 0;
}
