dump_config_as_events() {
    g_hash_table_foreach(uzbl.comm.proto_var, dump_var_hash_as_event, NULL);
}
