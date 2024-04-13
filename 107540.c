const char *pdf_get_err(struct pdf_doc *pdf, int *errval)
{
    if (!pdf)
        return NULL;
    if (pdf->errstr[0] == '\0')
        return NULL;
    if (errval) *errval = pdf->errval;
    return pdf->errstr;
}
