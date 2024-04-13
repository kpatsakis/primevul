XcursorImagesSetName (XcursorImages *images, const char *name)
{
    char    *new;

    if (!images || !name)
        return;

    new = strdup (name);

    if (!new)
	return;

    if (images->name)
	free (images->name);
    images->name = new;
}
