FontFileFreeDir (FontDirectoryPtr dir)
{
    FontFileFreeTable (&dir->scalable);
    FontFileFreeTable (&dir->nonScalable);
    free(dir);
}
