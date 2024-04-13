nsPluginInstance::nsPluginInstance(nsPluginCreateData* data)
    :
    nsPluginInstanceBase(),
    _instance(data->instance),
    _window(0),
    _width(0),
    _height(0),
    _streamfd(-1),
    _childpid(0),
    _filefd(-1),
    _name(),
    _scriptObject(0)
{

    for (size_t i=0, n=data->argc; i<n; ++i) {
        std::string name, val;
        gnash::StringNoCaseEqual noCaseCompare;

        if (data->argn[i]) {
            name = data->argn[i];
        }

        if (data->argv[i]) {
            val = data->argv[i];
        }

        if (noCaseCompare(name, "name")) {
            _name = val;
        }

        _params[name] = val;
    }

    if (HasScripting()) {
        _scriptObject = (GnashPluginScriptObject*) NPN_CreateObject( _instance,
            GnashPluginScriptObject::marshalGetNPClass());
    }
    
    return;
}
