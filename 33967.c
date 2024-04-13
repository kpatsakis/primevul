NS_PluginInitialize()
{

    if ( gnash::plugInitialized ) {
        gnash::log_debug("NS_PluginInitialize called, but ignored (we already initialized)");
        return NPERR_NO_ERROR;
    }

    gnash::log_debug("NS_PluginInitialize call ---------------------------");


    NPError err = NPERR_NO_ERROR;
    NPBool supportsXEmbed = TRUE;

    err = NPN_GetValue(NULL,NPNVSupportsXEmbedBool,
                       (void *)&supportsXEmbed);

    if (err != NPERR_NO_ERROR || !supportsXEmbed) {
        gnash::log_error("NPAPI ERROR: No xEmbed support in this browser!");
        return NPERR_INCOMPATIBLE_VERSION_ERROR;
    } else {
        gnash::log_debug("xEmbed supported in this browser");
    }

    NPNToolkitType toolkit;
    err = NPN_GetValue(NULL, NPNVToolkit, &toolkit);

    if (err != NPERR_NO_ERROR || toolkit != NPNVGtk2) {
        gnash::log_error("NPAPI ERROR: No GTK2 support in this browser! Have version %d", (int)toolkit);
    } else {
        gnash::log_debug("GTK2 supported in this browser");
    }

    /*
    Check for environment variables.
    */
    char* opts = std::getenv("GNASH_OPTIONS");
    if (opts != NULL) {
        gnash::log_debug("GNASH_OPTIONS: %s", opts);
        
        if ( strstr(opts, "waitforgdb") ) {
            waitforgdb = true;
        }

        if ( strstr(opts, "writelauncher") ) {
            createSaLauncher = true;
        }

    }


    std::string newGnashRc;

#if !defined(__OS2__ ) && ! defined(__amigaos4__)
    newGnashRc.append(SYSCONFDIR);
    newGnashRc.append("/gnashpluginrc");
#endif

    const char *home = NULL;
#if defined(__amigaos4__)
    home = "/gnash";
#elif defined(__HAIKU__)
    BPath bp;
    if (B_OK != find_directory(B_USER_SETTINGS_DIRECTORY, &bp))
    {
        std::cerr << "Failed to find user settings directory" << std::endl;
    } else {
        bp.Append("Gnash");
        home = bp.Path();
    }
#else
    home = std::getenv("HOME");
#endif
    if ( home ) {
        newGnashRc.append(":");
        newGnashRc.append(home);
#ifdef __HAIKU__
        newGnashRc.append("/gnashpluginrc");
#else
        newGnashRc.append("/.gnashpluginrc");
#endif
    } else {
        gnash::log_error("WARNING: NPAPI plugin could not find user home dir");
    }

    char *gnashrc = std::getenv("GNASHRC");
    if ( gnashrc ) {
        newGnashRc.append(":");
        newGnashRc.append(gnashrc);
    }

    if ( setenv("GNASHRC", newGnashRc.c_str(), 1) ) {
        gnash::log_debug("WARNING: NPAPI plugin could not append to the GNASHRC env variable");
    } else {
        gnash::log_debug("NOTE: NPAPI plugin set GNASHRC to %d", newGnashRc);
    }

    /* Success */

    gnash::plugInitialized = TRUE;

    return NPERR_NO_ERROR;
}
