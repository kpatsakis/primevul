kadm5_launch_task (krb5_context context,
                   const char *task_path, char * const task_argv[],
                   const char *buffer)
{
    kadm5_ret_t ret;
    int data_pipe[2];

    ret = pipe (data_pipe);
    if (ret)
        ret = errno;

    if (!ret) {
        pid_t pid = fork ();
        if (pid == -1) {
            ret = errno;
            close (data_pipe[0]);
            close (data_pipe[1]);
        } else if (pid == 0) {
            /* The child: */

            if (dup2 (data_pipe[0], STDIN_FILENO) == -1)
                _exit (1);

            close (data_pipe[0]);
            close (data_pipe[1]);

            execv (task_path, task_argv);

            _exit (1); /* Fail if execv fails */
        } else {
            /* The parent: */
            int status;

            ret = 0;

            close (data_pipe[0]);

            /* Write out the buffer to the child, add \n */
            if (buffer) {
                if (krb5_net_write (context, data_pipe[1], buffer, strlen (buffer)) < 0
                    || krb5_net_write (context, data_pipe[1], "\n", 1) < 0)
                {
                    /* kill the child to make sure waitpid() won't hang later */
                    ret = errno;
                    kill (pid, SIGKILL);
                }
            }
            close (data_pipe[1]);

            waitpid (pid, &status, 0);

            if (!ret) {
                if (WIFEXITED (status)) {
                    /* child read password and exited.  Check the return value. */
                    if ((WEXITSTATUS (status) != 0) && (WEXITSTATUS (status) != 252)) {
                        ret = KRB5KDC_ERR_POLICY; /* password change rejected */
                    }
                } else {
                    /* child read password but crashed or was killed */
                    ret = KRB5KRB_ERR_GENERIC; /* FIXME: better error */
                }
            }
        }
    }

    return ret;
}
