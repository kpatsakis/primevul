talk_to_socket(WebKitWebView *web_view, GArray *argv, GString *result) {
    (void)web_view; (void)result;

    int fd, len;
    struct sockaddr_un sa;
    char* sockpath;
    ssize_t ret;
    struct pollfd pfd;
    struct iovec* iov;
    guint i;

    if(uzbl.comm.sync_stdout) uzbl.comm.sync_stdout = strfree(uzbl.comm.sync_stdout);

    /* This function could be optimised by storing a hash table of socket paths
       and associated connected file descriptors rather than closing and
       re-opening for every call. Also we could launch a script if socket connect
       fails. */

    /* First element argv[0] is path to socket. Following elements are tokens to
       write to the socket. We write them as a single packet with each token
       separated by an ASCII nul (\0). */
    if(argv->len < 2) {
        g_printerr("talk_to_socket called with only %d args (need at least two).\n",
            (int)argv->len);
        return;
    }

    /* copy socket path, null terminate result */
    sockpath = g_array_index(argv, char*, 0);
    g_strlcpy(sa.sun_path, sockpath, sizeof(sa.sun_path));
    sa.sun_family = AF_UNIX;

    /* create socket file descriptor and connect it to path */
    fd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(fd == -1) {
        g_printerr("talk_to_socket: creating socket failed (%s)\n", strerror(errno));
        return;
    }
    if(connect(fd, (struct sockaddr*)&sa, sizeof(sa))) {
        g_printerr("talk_to_socket: connect failed (%s)\n", strerror(errno));
        close(fd);
        return;
    }

    /* build request vector */
    iov = g_malloc(sizeof(struct iovec) * (argv->len - 1));
    if(!iov) {
        g_printerr("talk_to_socket: unable to allocated memory for token vector\n");
        close(fd);
        return;
    }
    for(i = 1; i < argv->len; ++i) {
        iov[i - 1].iov_base = g_array_index(argv, char*, i);
        iov[i - 1].iov_len = strlen(iov[i - 1].iov_base) + 1; /* string plus \0 */
    }

    /* write request */
    ret = writev(fd, iov, argv->len - 1);
    g_free(iov);
    if(ret == -1) {
        g_printerr("talk_to_socket: write failed (%s)\n", strerror(errno));
        close(fd);
        return;
    }

    /* wait for a response, with a 500ms timeout */
    pfd.fd = fd;
    pfd.events = POLLIN;
    while(1) {
        ret = poll(&pfd, 1, 500);
        if(ret == 1) break;
        if(ret == 0) errno = ETIMEDOUT;
        if(errno == EINTR) continue;
        g_printerr("talk_to_socket: poll failed while waiting for input (%s)\n",
            strerror(errno));
        close(fd);
        return;
    }

    /* get length of response */
    if(ioctl(fd, FIONREAD, &len) == -1) {
        g_printerr("talk_to_socket: cannot find daemon response length, "
            "ioctl failed (%s)\n", strerror(errno));
        close(fd);
        return;
    }

    /* if there is a response, read it */
    if(len) {
        uzbl.comm.sync_stdout = g_malloc(len + 1);
        if(!uzbl.comm.sync_stdout) {
            g_printerr("talk_to_socket: failed to allocate %d bytes\n", len);
            close(fd);
            return;
        }
        uzbl.comm.sync_stdout[len] = 0; /* ensure result is null terminated */

        ret = read(fd, uzbl.comm.sync_stdout, len);
        if(ret == -1) {
            g_printerr("talk_to_socket: failed to read from socket (%s)\n",
                strerror(errno));
            close(fd);
            return;
        }
    }

    /* clean up */
    close(fd);
    return;
}
