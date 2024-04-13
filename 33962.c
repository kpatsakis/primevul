nsPluginInstance::GetValue(NPPVariable aVariable, void *aValue)
{

    if (aVariable == NPPVpluginScriptableNPObject) {
        if (_scriptObject) {
            void **v = (void **)aValue;
            NPN_RetainObject(_scriptObject);
            *v = _scriptObject;
        } else {
            gnash::log_debug("_scriptObject is not assigned");
        }
    }    


    return NS_PluginGetValue(aVariable, aValue);
}
