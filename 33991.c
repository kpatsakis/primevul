nsPluginInstance::~nsPluginInstance()
{

    if (_scriptObject) {
        NPN_ReleaseObject(_scriptObject);
    }

    do { } while (g_source_remove_by_user_data(this));

    if (_childpid > 0) {
        
        
        int status;
        int rv = waitpid(_childpid, &status, WNOHANG);

        if (rv <= 0) {
             int* pid = new int(_childpid);
             gnashSleep(1000);
	     cleanup_childpid(pid);
        } else {
            gnash::log_debug("Child process exited with status %d", status);
        }
    }
    _childpid = 0;
}
