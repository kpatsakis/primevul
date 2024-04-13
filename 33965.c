NS_NewPluginInstance(nsPluginCreateData * aCreateDataStruct)
{

    if(!aCreateDataStruct) {
        return NULL;
    }

    return new gnash::nsPluginInstance(aCreateDataStruct);
}
