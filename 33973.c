create_standalone_launcher(const std::string& page_url, const std::string& swf_url,
                           const std::map<std::string, std::string>& params)
{
#ifdef CREATE_STANDALONE_GNASH_LAUNCHER
    if (!createSaLauncher) {
        return;
    }

    std::ofstream saLauncher;

    std::stringstream ss;
    static int debugno = 0;
    debugno = (debugno + 1) % 10;
    ss << "/tmp/gnash-debug-" << debugno << ".sh";
    saLauncher.open(ss.str().c_str(), std::ios::out | std::ios::trunc);

    if (!saLauncher) {
        gnash::log_error("Failed to open new file for standalone launcher: " + ss.str());
        return;
    }

    saLauncher << "#!/bin/sh" << std::endl
               << "export GNASH_COOKIES_IN="
               << "/tmp/gnash-cookies." << getpid() << std::endl
               << getGnashExecutable() << " ";

    if (!page_url.empty()) {
        saLauncher << "-U '" << page_url << "' ";
    }

    for (std::map<std::string,std::string>::const_iterator it = params.begin(),
        itEnd = params.end(); it != itEnd; ++it) {
        const std::string& nam = it->first; 
        const std::string& val = it->second;
        saLauncher << "-P '"
                   << boost::algorithm::replace_all_copy(nam, "'", "'\\''")
                   << "="
                   << boost::algorithm::replace_all_copy(val, "'", "'\\''")
                   << "' ";
    }

    saLauncher << "'" << swf_url << "' "
               << "$@"       // allow caller to pass any additional argument
               << std::endl;

    saLauncher.close();
#endif
}
