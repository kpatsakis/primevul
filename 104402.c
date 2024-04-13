static void kbd_leds(void *opaque, int ledstate)
{
    VncState *vs = opaque;
    int caps, num, scr;
    bool has_changed = (ledstate != current_led_state(vs));

    trace_vnc_key_guest_leds((ledstate & QEMU_CAPS_LOCK_LED),
                             (ledstate & QEMU_NUM_LOCK_LED),
                             (ledstate & QEMU_SCROLL_LOCK_LED));

    caps = ledstate & QEMU_CAPS_LOCK_LED ? 1 : 0;
    num  = ledstate & QEMU_NUM_LOCK_LED  ? 1 : 0;
    scr  = ledstate & QEMU_SCROLL_LOCK_LED ? 1 : 0;

    if (vs->modifiers_state[0x3a] != caps) {
        vs->modifiers_state[0x3a] = caps;
    }
    if (vs->modifiers_state[0x45] != num) {
        vs->modifiers_state[0x45] = num;
    }
    if (vs->modifiers_state[0x46] != scr) {
        vs->modifiers_state[0x46] = scr;
    }

    /* Sending the current led state message to the client */
    if (has_changed) {
        vnc_led_state_change(vs);
    }
}
