void pdf_destroy(struct pdf_doc *pdf)
{
    if (pdf) {
        int i;
        for (i = 0; i < flexarray_size(&pdf->objects); i++)
            pdf_object_destroy(pdf_get_object(pdf, i));
        flexarray_clear(&pdf->objects);
        free(pdf);
    }
}
