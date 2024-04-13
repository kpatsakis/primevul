nsPluginInstance::setupIOChannel(int fd, GIOFunc handler, GIOCondition signals) const
{
    GIOChannel* ichan = g_io_channel_unix_new(fd);
    g_io_channel_set_close_on_unref(ichan, true);

    gnash::log_debug("New IO Channel for fd #%d",
                     g_io_channel_unix_get_fd(ichan));
    g_io_add_watch(ichan, signals, handler, (gpointer)this); 
    g_io_channel_unref(ichan);
}
