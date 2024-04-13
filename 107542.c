static struct pdf_object *pdf_get_object(struct pdf_doc *pdf, int index)
{
    return flexarray_get(&pdf->objects, index);
}
