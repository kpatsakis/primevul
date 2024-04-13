static bool megasas_use_msix(MegasasState *s)
{
    return s->flags & MEGASAS_MASK_USE_MSIX;
}
