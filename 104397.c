static int current_led_state(VncState *vs)
{
    int ledstate = 0;

    if (vs->modifiers_state[0x46]) {
        ledstate |= QEMU_SCROLL_LOCK_LED;
    }
    if (vs->modifiers_state[0x45]) {
        ledstate |= QEMU_NUM_LOCK_LED;
    }
    if (vs->modifiers_state[0x3a]) {
        ledstate |= QEMU_CAPS_LOCK_LED;
    }

    return ledstate;
}
