static int pdf_add_stream(struct pdf_doc *pdf, struct pdf_object *page,
                          char *buffer)
{
    struct pdf_object *obj;
    int len;
    char prefix[128];
    char suffix[128];

    if (!page)
        page = pdf_find_last_object(pdf, OBJ_page);

    if (!page)
        return pdf_set_err(pdf, -EINVAL, "Invalid pdf page");

    len = strlen(buffer);
    /* We don't want any trailing whitespace in the stream */
    while (len >= 1 && (buffer[len - 1] == '\r' ||
                        buffer[len - 1] == '\n')) {
        buffer[len - 1] = '\0';
        len--;
    }

    sprintf(prefix, "<< /Length %d >>stream\r\n", len);
    sprintf(suffix, "\r\nendstream\r\n");
    len += strlen(prefix) + strlen(suffix);

    obj = pdf_add_object(pdf, OBJ_stream);
    if (!obj)
        return pdf->errval;
    obj->stream.text = malloc(len + 1);
    if (!obj->stream.text) {
        obj->type = OBJ_none;
        return pdf_set_err(pdf, -ENOMEM, "Insufficient memory for text (%d bytes)",
                           len + 1);
    }
    obj->stream.text[0] = '\0';
    strcat(obj->stream.text, prefix);
    strcat(obj->stream.text, buffer);
    strcat(obj->stream.text, suffix);
    obj->stream.len = 0;

    return flexarray_append(&page->page.children, obj);
}
