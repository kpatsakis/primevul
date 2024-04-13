getPluginDescription() 
{
    static const char* desc = NULL;
    if (!desc) {
        desc = std::getenv("GNASH_PLUGIN_DESCRIPTION");
        if (desc == NULL) desc = PLUGIN_DESCRIPTION;
    }
    return desc;
}
