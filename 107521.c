int pdf_add_jpeg(struct pdf_doc *pdf, struct pdf_object *page,
                 int x, int y, int display_width, int display_height,
                 const char *jpeg_file)
{
    struct pdf_object *obj;

    obj = pdf_add_raw_jpeg(pdf, jpeg_file);
    if (!obj)
        return pdf->errval;

    return pdf_add_image(pdf, page, obj, x, y, display_width, display_height);
}
