static bool megasas_use_msi(MegasasState *s)
{
    return s->flags & MEGASAS_MASK_USE_MSI;
}
