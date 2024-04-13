pvscsi_msg_ring_put(PVSCSIState *s, struct PVSCSIRingMsgDesc *msg_desc)
{
    hwaddr msg_descr_pa;

    msg_descr_pa = pvscsi_ring_pop_msg_descr(&s->rings);
    trace_pvscsi_msg_ring_put(msg_descr_pa);
    cpu_physical_memory_write(msg_descr_pa, (void *)msg_desc,
                              sizeof(*msg_desc));
}
