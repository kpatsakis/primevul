static int mptsas_phy_addr_get(MPTSASState *s, int address)
{
    int i;
    if ((address >> MPI_SAS_PHY_PGAD_FORM_SHIFT) == 0) {
        i = address & 255;
    } else if ((address >> MPI_SAS_PHY_PGAD_FORM_SHIFT) == 1) {
        i = address & 65535;
    } else {
        return -EINVAL;
    }

    if (i >= MPTSAS_NUM_PORTS) {
        return -EINVAL;
    }

    return i;
}
