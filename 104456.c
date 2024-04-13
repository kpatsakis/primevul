int vnc_display_password(DisplayState *ds, const char *password)
{
    VncDisplay *vs = vnc_display;

    if (!vs) {
        return -EINVAL;
    }
    if (vs->auth == VNC_AUTH_NONE) {
        error_printf_unless_qmp("If you want use passwords please enable "
                                "password auth using '-vnc ${dpy},password'.");
        return -EINVAL;
    }

    g_free(vs->password);
    vs->password = g_strdup(password);

    return 0;
}
