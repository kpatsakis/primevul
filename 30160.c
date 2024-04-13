FontFileFreeEntry (FontEntryPtr entry)
{
    FontScalableExtraPtr   extra;
    int i;

    if (entry->name.name)
	free(entry->name.name);
    entry->name.name = NULL;

    switch (entry->type)
    {
    case FONT_ENTRY_SCALABLE:
	free (entry->u.scalable.fileName);
	extra = entry->u.scalable.extra;
	for (i = 0; i < extra->numScaled; i++)
	    if (extra->scaled[i].vals.ranges)
		free (extra->scaled[i].vals.ranges);
	free (extra->scaled);
	free (extra);
	break;
    case FONT_ENTRY_BITMAP:
	free (entry->u.bitmap.fileName);
	entry->u.bitmap.fileName = NULL;
	break;
    case FONT_ENTRY_ALIAS:
	free (entry->u.alias.resolved);
	entry->u.alias.resolved = NULL;
	break;
    }
}
