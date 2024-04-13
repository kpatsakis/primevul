static struct pdf_object *pdf_find_last_object(struct pdf_doc *pdf,
        int type)
{
    return pdf->last_objects[type];
}
