static void usb_net_reset_in_buf(USBNetState *s)
{
    s->in_ptr = s->in_len = 0;
    qemu_flush_queued_packets(qemu_get_queue(s->nic));
}
