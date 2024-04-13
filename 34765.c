CatalogueNameCheck (char *name)
{
    return strncmp(name, CataloguePrefix, sizeof(CataloguePrefix) - 1) == 0;
}
