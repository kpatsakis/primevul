static void mcf_fec_write_bd(mcf_fec_bd *bd, uint32_t addr)
{
    mcf_fec_bd tmp;
    tmp.flags = cpu_to_be16(bd->flags);
    tmp.length = cpu_to_be16(bd->length);
    tmp.data = cpu_to_be32(bd->data);
    cpu_physical_memory_write(addr, &tmp, sizeof(tmp));
}
