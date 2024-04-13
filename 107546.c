int pdf_save(struct pdf_doc *pdf, const char *filename)
{
    FILE *fp;
    int i;
    struct pdf_object *obj;
    int xref_offset;
    int xref_count = 0;

    if (filename == NULL)
        fp = stdout;
    else if ((fp = fopen(filename, "wb")) == NULL)
        return pdf_set_err(pdf, -errno, "Unable to open '%s': %s",
                           filename, strerror(errno));

    fprintf(fp, "%%PDF-1.2\r\n");
    /* Hibit bytes */
    fprintf(fp, "%c%c%c%c%c\r\n", 0x25, 0xc7, 0xec, 0x8f, 0xa2);

    /* Dump all the objects & get their file offsets */
    for (i = 0; i < flexarray_size(&pdf->objects); i++)
        if (pdf_save_object(pdf, fp, i) >= 0)
            xref_count++;

    /* xref */
    xref_offset = ftell(fp);
    fprintf(fp, "xref\r\n");
    fprintf(fp, "0 %d\r\n", xref_count + 1);
    fprintf(fp, "0000000000 65535 f\r\n");
    for (i = 0; i < flexarray_size(&pdf->objects); i++) {
        obj = pdf_get_object(pdf, i);
        if (obj->type != OBJ_none)
            fprintf(fp, "%10.10d 00000 n\r\n",
                    obj->offset);
    }

    fprintf(fp, "trailer\r\n"
            "<<\r\n"
            "/Size %d\r\n", xref_count + 1);
    obj = pdf_find_first_object(pdf, OBJ_catalog);
    fprintf(fp, "/Root %d 0 R\r\n", obj->index);
    obj = pdf_find_first_object(pdf, OBJ_info);
    fprintf(fp, "/Info %d 0 R\r\n", obj->index);
    /* FIXME: Not actually generating a unique ID */
    fprintf(fp, "/ID [<%16.16x> <%16.16x>]\r\n", 0x123, 0x123);
    fprintf(fp, ">>\r\n"
            "startxref\r\n");
    fprintf(fp, "%d\r\n", xref_offset);
    fprintf(fp, "%%%%EOF\r\n");
    fclose(fp);

    return 0;
}
