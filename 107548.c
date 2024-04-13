static int pdf_set_err(struct pdf_doc *doc, int errval,
                       const char *buffer, ...)
{
    va_list ap;
    int len;

    va_start(ap, buffer);
    len = vsnprintf(doc->errstr, sizeof(doc->errstr) - 2, buffer, ap);
    va_end(ap);

    /* Make sure we're properly terminated */
    if (doc->errstr[len] != '\n')
        doc->errstr[len] = '\n';
    doc->errstr[len] = '\0';
    doc->errval = errval;

    return errval;
}
