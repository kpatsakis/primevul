nsPluginInstance::SetWindow(NPWindow* aWindow)
{
    if(!aWindow) {
        gnash::log_error(std::string(__FUNCTION__) + ": ERROR: Window handle was bogus!");
        return NPERR_INVALID_PARAM;
#if 0
    } else {
        gnash::log_debug("%s: X origin = %d, Y Origin = %d, Width = %d,"
            " Height = %d, WindowID = %p, this = %p",
            __FUNCTION__,
            aWindow->x, aWindow->y, aWindow->width, aWindow->height,
            aWindow->window, this);
#endif
    }

    if (_window) {
        return NPERR_GENERIC_ERROR;
    }

    _width = aWindow->width;
    _height = aWindow->height;

    _window = reinterpret_cast<Window> (aWindow->window);

    if (!_childpid && !_swf_url.empty()) {
        return startProc();
    }

    return NPERR_NO_ERROR;
}
