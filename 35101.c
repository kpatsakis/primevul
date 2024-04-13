event_fixture_teardown(struct EventFixture *ef, const void *data)
{
    (void) data;

    /* there should be no events left waiting */
    assert_no_event(ef);

    /* clean up the io channel we opened for uzbl */
    GIOChannel *iochan = g_ptr_array_index(uzbl.comm.client_chan, 0);
    remove_socket_from_array(iochan);

    /* close the sockets so that nothing sticks around between tests */
    close(ef->uzbl_sock);
    close(ef->test_sock);
}
