nsPluginInstance::shut()
{
    gnash::log_debug("Gnash plugin shutting down");

    if (_streamfd != -1) {
        if (close(_streamfd) == -1) {
            perror("closing _streamfd");
        } else {
            _streamfd = -1;
        }
    }

}
