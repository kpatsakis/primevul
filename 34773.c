ComparePriority(const void *p1, const void *p2)
{
    FontDirectoryPtr dir1 = (*(FontPathElementPtr*) p1)->private;
    FontDirectoryPtr dir2 = (*(FontPathElementPtr*) p2)->private;
    const char *pri1 = NULL;
    const char *pri2 = NULL;

    if (dir1->attributes != NULL)
	pri1 = strstr(dir1->attributes, PriorityAttribute);
    if (dir2->attributes != NULL)
	pri2 = strstr(dir2->attributes, PriorityAttribute);

    if (pri1 == NULL && pri2 == NULL)
	return 0;
    else if (pri1 == NULL)
	return 1;
    else if (pri2 == NULL)
	return -1;
    else
	return
	    atoi(pri1 + strlen(PriorityAttribute)) -
	    atoi(pri2 + strlen(PriorityAttribute));
}
