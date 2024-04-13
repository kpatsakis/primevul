void vnc_display_init(DisplayState *ds)
{
    VncDisplay *vs = g_malloc0(sizeof(*vs));

    vnc_display = vs;

    vs->lsock = -1;
#ifdef CONFIG_VNC_WS
    vs->lwebsock = -1;
#endif

    QTAILQ_INIT(&vs->clients);
    vs->expires = TIME_MAX;

    if (keyboard_layout) {
        trace_vnc_key_map_init(keyboard_layout);
        vs->kbd_layout = init_keyboard_layout(name2keysym, keyboard_layout);
    } else {
        vs->kbd_layout = init_keyboard_layout(name2keysym, "en-us");
    }

    if (!vs->kbd_layout)
        exit(1);

    qemu_mutex_init(&vs->mutex);
    vnc_start_worker_thread();

    vs->dcl.ops = &dcl_ops;
    register_displaychangelistener(&vs->dcl);
}
