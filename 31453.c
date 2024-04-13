static void single_inst_client_free(SingleInstClient* client)
{
    g_io_channel_shutdown(client->channel, FALSE, NULL);
    g_io_channel_unref(client->channel);
    g_source_remove(client->watch);
    g_free(client->cwd);
    g_ptr_array_foreach(client->argv, (GFunc)g_free, NULL);
    g_ptr_array_free(client->argv, TRUE);
    g_slice_free(SingleInstClient, client);
    /* g_debug("free client"); */
}
