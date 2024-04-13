remove_socket_from_array(GIOChannel *chan) {
    gboolean ret = 0;

    /* TODO: Do wee need to manually free the IO channel or is this
     *       happening implicitly on unref?
     */
    ret = g_ptr_array_remove_fast(uzbl.comm.connect_chan, chan);
    if(!ret)
        ret = g_ptr_array_remove_fast(uzbl.comm.client_chan, chan);

    return ret;
}
