static void lsi_resume_script(LSIState *s)
{
    if (s->waiting != 2) {
        s->waiting = LSI_NOWAIT;
        lsi_execute_script(s);
    } else {
        s->waiting = LSI_NOWAIT;
    }
}
