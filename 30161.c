FontFileFreeTable (FontTablePtr table)
{
    int	i;

    for (i = 0; i < table->used; i++)
	FontFileFreeEntry (&table->entries[i]);
    free (table->entries);
}
