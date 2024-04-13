static struct pdf_object *pdf_find_first_object(struct pdf_doc *pdf,
        int type)
{
    return pdf->first_objects[type];
}
