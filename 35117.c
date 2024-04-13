catch_signal(int s) {
    if(s == SIGTERM ||
       s == SIGINT  ||
       s == SIGILL  ||
       s == SIGFPE  ||
       s == SIGQUIT) {
        clean_up();
        exit(EXIT_SUCCESS);
    }
    else if(s == SIGSEGV) {
        clean_up();
        fprintf(stderr, "Program aborted, segmentation fault!\nAttempting to clean up...\n");
        exit(EXIT_FAILURE);
    }
    else if(s == SIGALRM && uzbl.state.event_buffer) {
        g_ptr_array_free(uzbl.state.event_buffer, TRUE);
        uzbl.state.event_buffer = NULL;
    }
}
