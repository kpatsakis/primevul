pvscsi_cmp_ring_put(PVSCSIState *s, struct PVSCSIRingCmpDesc *cmp_desc)
{
    hwaddr cmp_descr_pa;

    cmp_descr_pa = pvscsi_ring_pop_cmp_descr(&s->rings);
    trace_pvscsi_cmp_ring_put(cmp_descr_pa);
    cpu_physical_memory_write(cmp_descr_pa, (void *)cmp_desc,
                              sizeof(*cmp_desc));
}
