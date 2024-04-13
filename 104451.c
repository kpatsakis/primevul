void vnc_display_add_client(DisplayState *ds, int csock, bool skipauth)
{
    VncDisplay *vs = vnc_display;

    vnc_connect(vs, csock, skipauth, false);
}
