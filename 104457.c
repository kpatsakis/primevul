int vnc_display_pw_expire(DisplayState *ds, time_t expires)
{
    VncDisplay *vs = vnc_display;

    if (!vs) {
        return -EINVAL;
    }

    vs->expires = expires;
    return 0;
}
