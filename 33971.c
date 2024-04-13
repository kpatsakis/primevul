cleanup_childpid(gpointer data)
{
    int* pid = static_cast<int*>(data);

    int status;
    int rv = waitpid(*pid, &status, WNOHANG);

    if (rv <= 0) {

        kill(*pid, SIGKILL);
        waitpid(*pid, &status, 0);
    }
 
    gnash::log_debug("Child process exited with status %s", status);

    delete pid;

    return FALSE;
}
