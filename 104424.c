void start_client_init(VncState *vs)
{
    vnc_read_when(vs, protocol_client_init, 1);
}
