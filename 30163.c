FontFileMakeDir(const char *dirName, int size)
{
    FontDirectoryPtr	dir;
    int			dirlen;
    int			needslash = 0;
    const char		*attrib;
    int			attriblen;

#if !defined(WIN32)
    attrib = strchr(dirName, ':');
#else
    /* OS/2 uses the colon in the drive letter descriptor, skip this */
    attrib = strchr(dirName+2, ':');
#endif
    if (attrib) {
	dirlen = attrib - dirName;
	attriblen = strlen(attrib);
    } else {
	dirlen = strlen(dirName);
	attriblen = 0;
    }
    if (dirName[dirlen - 1] != '/')
#ifdef NCD
    if (dirlen)     /* leave out slash for builtins */
#endif
	needslash = 1;
    dir = malloc(sizeof *dir + dirlen + needslash + 1 +
		 (attriblen ? attriblen + 1 : 0));
    if (!dir)
	return (FontDirectoryPtr)0;
    if (!FontFileInitTable (&dir->scalable, 0))
    {
	free (dir);
	return (FontDirectoryPtr)0;
    }
    if (!FontFileInitTable (&dir->nonScalable, size))
    {
	FontFileFreeTable (&dir->scalable);
	free (dir);
	return (FontDirectoryPtr)0;
    }
    dir->directory = (char *) (dir + 1);
    dir->dir_mtime = 0;
    dir->alias_mtime = 0;
    if (attriblen)
	dir->attributes = dir->directory + dirlen + needslash + 1;
    else
	dir->attributes = NULL;
    strncpy(dir->directory, dirName, dirlen);
    dir->directory[dirlen] = '\0';
    if (dir->attributes)
	strcpy(dir->attributes, attrib);
    if (needslash)
	strcat(dir->directory, "/");
    return dir;
}
