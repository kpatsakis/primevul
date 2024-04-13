clean_up(void) {
    send_event(INSTANCE_EXIT, uzbl.info.pid_str, NULL);
    g_free(uzbl.info.pid_str);

    g_free(uzbl.state.executable_path);
    g_hash_table_destroy(uzbl.behave.commands);

    if(uzbl.state.event_buffer)
        g_ptr_array_free(uzbl.state.event_buffer, TRUE);

    if (uzbl.behave.fifo_dir)
        unlink (uzbl.comm.fifo_path);
    if (uzbl.behave.socket_dir)
        unlink (uzbl.comm.socket_path);
}
