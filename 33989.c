nsPluginInstance::startProc()
{

    int p2c_pipe[2];
    int c2p_pipe[2];
    int p2c_controlpipe[2];

    int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, p2c_pipe);
    if (ret == -1) {
        gnash::log_error("socketpair(p2c) failed: %s", strerror(errno));
        return NPERR_GENERIC_ERROR;
    }
    _streamfd = p2c_pipe[1];

    ret = socketpair(AF_UNIX, SOCK_STREAM, 0, c2p_pipe);
    if (ret == -1) {
        gnash::log_error("socketpair(c2p) failed: %s", strerror(errno));
        return NPERR_GENERIC_ERROR;
    }

    ret = socketpair(AF_UNIX, SOCK_STREAM, 0, p2c_controlpipe);
    if (ret == -1) {
        gnash::log_error("socketpair(control) failed: %s", strerror(errno));
        return NPERR_GENERIC_ERROR;
    }

    if (HasScripting() && _scriptObject) {
        _scriptObject->setControlFD(p2c_controlpipe[1]);
        _scriptObject->setHostFD(c2p_pipe[0]);
    }
    

    std::vector<std::string> arg_vec = getCmdLine(c2p_pipe[1],
                                                  p2c_controlpipe[0]);

    if (arg_vec.empty()) {
        gnash::log_error("Failed to obtain command line parameters.");
        return NPERR_GENERIC_ERROR;
    }
    
    std::vector<const char*> args;
    
    std::transform(arg_vec.begin(), arg_vec.end(), std::back_inserter(args),
                   std::mem_fun_ref(&std::string::c_str));
    args.push_back(0);
    
    
    _childpid = fork();
    
    if (_childpid == -1) {
        gnash::log_error("fork() failed: %s", strerror(errno));
        return NPERR_OUT_OF_MEMORY_ERROR;
    }
    
    if (_childpid > 0) {
        
        ret = close (p2c_pipe[0]);
        if (ret == -1) {
            gnash::log_error("p2c_pipe[0] close() failed: %s",
                             strerror(errno));
        }
        
        ret = close (c2p_pipe[1]);
        if (ret == -1) {
            gnash::log_error("c2p_pipe[1] close() failed: %s",
                             strerror(errno));
            gnash::log_debug("Forked successfully but with ignorable errors.");
        } else {
            gnash::log_debug("Forked successfully, child process PID is %d",
                             _childpid);
        }

        setupIOChannel(c2p_pipe[0], (GIOFunc)handlePlayerRequestsWrapper,
                                    (GIOCondition)(G_IO_IN|G_IO_HUP));
        
        setupIOChannel(p2c_controlpipe[1], remove_handler, G_IO_HUP);

        return NPERR_NO_ERROR;
    }
    
    
    ret = close (p2c_pipe[1]);
    
    ret = dup2 (p2c_pipe[0], fileno(stdin));

    if (ret == -1) {
        gnash::log_error("dup2() failed: %s", strerror(errno));
    }
    
    int dontclose[] = {c2p_pipe[1], c2p_pipe[0], p2c_controlpipe[0]};
    close_fds(dontclose);

    
    gnash::log_debug("Starting process: %s", boost::algorithm::join(arg_vec, " "));

    wait_for_gdb();

    execv(args[0], const_cast<char**>(&args.front()));

    perror("executing standalone gnash");
    
    exit (-1);
}
