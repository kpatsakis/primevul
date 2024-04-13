static int mptsas_device_addr_get(MPTSASState *s, int address)
{
    uint32_t handle, i;
    uint32_t form = address >> MPI_SAS_PHY_PGAD_FORM_SHIFT;
    if (form == MPI_SAS_DEVICE_PGAD_FORM_GET_NEXT_HANDLE) {
        handle = address & MPI_SAS_DEVICE_PGAD_GNH_HANDLE_MASK;
        do {
            if (handle == 65535) {
                handle = MPTSAS_NUM_PORTS + 1;
            } else {
                ++handle;
            }
            i = handle - 1 - MPTSAS_NUM_PORTS;
        } while (i < MPTSAS_NUM_PORTS && !scsi_device_find(&s->bus, 0, i, 0));

    } else if (form == MPI_SAS_DEVICE_PGAD_FORM_BUS_TARGET_ID) {
        if (address & MPI_SAS_DEVICE_PGAD_BT_BUS_MASK) {
            return -EINVAL;
        }
        i = address & MPI_SAS_DEVICE_PGAD_BT_TID_MASK;

    } else if (form == MPI_SAS_DEVICE_PGAD_FORM_HANDLE) {
        handle = address & MPI_SAS_DEVICE_PGAD_H_HANDLE_MASK;
        i = handle - 1 - MPTSAS_NUM_PORTS;

    } else {
        return -EINVAL;
    }

    if (i >= MPTSAS_NUM_PORTS) {
        return -EINVAL;
    }

    return i;
}
