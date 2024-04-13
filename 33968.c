nsPluginInstance::NewStream(NPMIMEType /*type*/, NPStream* stream,
                            NPBool /*seekable*/, uint16_t* /*stype*/)
{

    if (_childpid) {
        return NPERR_GENERIC_ERROR;
    }
    _swf_url = stream->url;

    if (!_swf_url.empty() && _window) {
        return startProc();
    }

    return NPERR_NO_ERROR;
}
