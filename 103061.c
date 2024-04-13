static void lsi_stop_script(LSIState *s)
{
    s->istat1 &= ~LSI_ISTAT1_SRUN;
}
