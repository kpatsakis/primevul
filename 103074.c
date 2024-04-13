static gboolean channel_init(GAState *s, const gchar *method, const gchar *path)
{
    GAChannelMethod channel_method;

    if (method == NULL) {
        method = "virtio-serial";
    }

    if (path == NULL) {
        if (strcmp(method, "virtio-serial") != 0) {
            g_critical("must specify a path for this channel");
            return false;
        }
        /* try the default path for the virtio-serial port */
        path = QGA_VIRTIO_PATH_DEFAULT;
    }

    if (strcmp(method, "virtio-serial") == 0) {
        s->virtio = true; /* virtio requires special handling in some cases */
        channel_method = GA_CHANNEL_VIRTIO_SERIAL;
    } else if (strcmp(method, "isa-serial") == 0) {
        channel_method = GA_CHANNEL_ISA_SERIAL;
    } else if (strcmp(method, "unix-listen") == 0) {
        channel_method = GA_CHANNEL_UNIX_LISTEN;
    } else {
        g_critical("unsupported channel method/type: %s", method);
        return false;
    }

    s->channel = ga_channel_new(channel_method, path, channel_event_cb, s);
    if (!s->channel) {
        g_critical("failed to create guest agent channel");
        return false;
    }

    return true;
}
