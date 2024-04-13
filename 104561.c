static void mcf_fec_read_bd(mcf_fec_bd *bd, uint32_t addr)
{
    cpu_physical_memory_read(addr, bd, sizeof(*bd));
    be16_to_cpus(&bd->flags);
    be16_to_cpus(&bd->length);
    be32_to_cpus(&bd->data);
}
