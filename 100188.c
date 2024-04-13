static int getdatetime(time_t *date)
{
    int c;
    int r;
    int i = 0;
    char buf[RFC3501_DATETIME_MAX+1];

    c = prot_getc(imapd_in);
    if (c != '\"')
        goto baddate;
    while ((c = prot_getc(imapd_in)) != '\"') {
        if (i >= RFC3501_DATETIME_MAX)
            goto baddate;
        buf[i++] = c;
    }
    buf[i] = '\0';

    r = time_from_rfc3501(buf, date);
    if (r < 0)
        goto baddate;

    c = prot_getc(imapd_in);
    return c;

 baddate:
    prot_ungetc(c, imapd_in);
    return EOF;
}
