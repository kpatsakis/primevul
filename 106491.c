XcursorShapeLoadImages (unsigned int shape, const char *theme, int size)
{
    unsigned int    id = shape >> 1;

    if (id < NUM_STANDARD_NAMES)
	return XcursorLibraryLoadImages (STANDARD_NAME (id), theme, size);
    else
	return NULL;
}
