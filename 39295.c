readextension(void)
{
    int count;
    char buf[255];

    (void) getc(infile);
    while ((count = getc(infile)))
        fread(buf, 1, count, infile);
}
