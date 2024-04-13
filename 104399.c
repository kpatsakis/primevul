static void ext_key_event(VncState *vs, int down,
                          uint32_t sym, uint16_t keycode)
{
    /* if the user specifies a keyboard layout, always use it */
    if (keyboard_layout) {
        key_event(vs, down, sym);
    } else {
        trace_vnc_key_event_ext(down, sym, keycode, code2name(keycode));
        do_key_event(vs, down, keycode, sym);
    }
}
