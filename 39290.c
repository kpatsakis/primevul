convert(void)
{
    int ch;
    char* mode = "w";

    if (!checksignature())
        return (-1);
    readscreen();
    while ((ch = getc(infile)) != ';' && ch != EOF) {
        switch (ch) {
            case '\0':  break;  /* this kludge for non-standard files */
            case ',':   if (!readgifimage(mode))
                           return (-1);
			mode = "a";		/* subsequent images append */
                        break;
            case '!':   readextension();
                        break;
            default:    fprintf(stderr, "illegal GIF block type\n");
                        return (-1);
        }
    }
    return (0);
}
