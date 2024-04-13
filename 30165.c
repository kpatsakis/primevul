FontFileSortTable (FontTablePtr table)
{
    if (!table->sorted) {
	qsort((char *) table->entries, table->used, sizeof(FontEntryRec),
	      FontFileNameCompare);
	table->sorted = TRUE;
    }
}
