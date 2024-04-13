nsPluginInstance::setupProxy(const std::string& url)
{
#if NPAPI_VERSION != 190
    if (!NPNFuncs.getvalueforurl) return;
#endif

    char *proxy = 0;
    uint32_t length = 0;
#if NPAPI_VERSION != 190
    NPN_GetValueForURL(_instance, NPNURLVProxy, url.c_str(),
                       &proxy, &length);
#endif
    if (!proxy) {
        gnash::log_debug("No proxy setting for %s", url);
        return;
    }

    std::string nproxy (proxy, length);
    NPN_MemFree(proxy);

    gnash::log_debug("Proxy setting for %s is %s", url, nproxy);

    std::vector<std::string> parts;
    boost::split(parts, nproxy,
        boost::is_any_of(" "), boost::token_compress_on);
    if ( parts[0] == "DIRECT" ) {
    }
    else if ( parts[0] == "PROXY" ) {
        if (setenv("http_proxy", parts[1].c_str(), 1) < 0) {
            gnash::log_error(
                "Couldn't set environment variable http_proxy to %s",
                nproxy);
        }
    }
    else {
        gnash::log_error("Unknown proxy type: %s", nproxy);
    }

}
