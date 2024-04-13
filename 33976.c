getGnashExecutable()
{
    std::string procname;
    bool process_found = false;
    struct stat procstats;

    char *gnash_env = std::getenv("GNASH_PLAYER");

    if (gnash_env) {
        procname = gnash_env;
        process_found = (0 == stat(procname.c_str(), &procstats));
        if (!process_found) {
            gnash::log_error("Invalid path to gnash executable: ");
            return "";
        }
    }

    if (!process_found) {
        procname = GNASHBINDIR "/gtk-gnash";
        process_found = (0 == stat(procname.c_str(), &procstats));
    }
    if (!process_found) {
        procname = GNASHBINDIR "/qt4-gnash";
        process_found = (0 == stat(procname.c_str(), &procstats));
    }

    if (!process_found) {
        gnash::log_error(std::string("Unable to find Gnash in ") + GNASHBINDIR);
        return "";
    }

    return procname;
}
