static void flush_queued_data_bh(void *opaque)
{
    VirtIOSerialPort *port = opaque;

    flush_queued_data(port);
}