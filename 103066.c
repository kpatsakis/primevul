static const char *scsi_phase_name(int phase)
{
    return scsi_phases[phase & PHASE_MASK];
}
