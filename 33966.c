NS_PluginGetValue(NPPVariable aVariable, void *aValue)
{
    NPError err = NPERR_NO_ERROR;

    switch (aVariable) {
      case NPPVpluginNameString:
          *static_cast<const char **> (aValue) = PLUGIN_NAME;
          break;
          
      case NPPVpluginDescriptionString:
          *static_cast<const char **>(aValue) = getPluginDescription();
          break;

      case NPPVpluginWindowBool:
          break;
          
      case NPPVpluginTimerInterval:
          break;
          
      case NPPVpluginKeepLibraryInMemory:
          break;
          
      case NPPVpluginNeedsXEmbed:
#ifdef HAVE_GTK2
          *static_cast<NPBool *>(aValue) = TRUE;
#else
          *static_cast<NPBool *>(aValue) = FALSE;
#endif
          break;

      case NPPVpluginScriptableNPObject:
          break;

#if NPAPI_VERSION != 190
      case NPPVpluginUrlRequestsDisplayedBool:
          break;
      case NPPVpluginWantsAllNetworkStreams:
          break;
#endif
          
      default:
          err = NPERR_INVALID_PARAM;
          break;
    }
    return err;
}
