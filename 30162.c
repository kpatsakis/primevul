FontFileInitTable (FontTablePtr table, int size)
{
    if (size < 0 || (size > INT32_MAX/sizeof(FontEntryRec)))
	return FALSE;
    if (size)
    {
	table->entries = malloc(sizeof(FontEntryRec) * size);
	if (!table->entries)
	    return FALSE;
    }
    else
	table->entries = 0;
    table->used = 0;
    table->size = size;
    table->sorted = FALSE;
    return TRUE;
}
