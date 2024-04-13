create_plug () {
    GtkPlug* plug = GTK_PLUG (gtk_plug_new (uzbl.state.socket_id));
    g_signal_connect (G_OBJECT (plug), "destroy", G_CALLBACK (destroy_cb), NULL);
    g_signal_connect (G_OBJECT (plug), "key-press-event", G_CALLBACK (key_press_cb), NULL);
    g_signal_connect (G_OBJECT (plug), "key-release-event", G_CALLBACK (key_release_cb), NULL);

    return plug;
}
