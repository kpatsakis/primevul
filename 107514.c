int pdf_add_barcode(struct pdf_doc *pdf, struct pdf_object *page,
                    int code, int x, int y, int width, int height,
                    const char *string, uint32_t colour)
{
    if (!string || !*string)
        return 0;
    switch (code) {
    case PDF_BARCODE_128A:
        return pdf_add_barcode_128a(pdf, page, x, y,
                                    width, height, string, colour);
    case PDF_BARCODE_39:
        return pdf_add_barcode_39(pdf, page, x, y, width, height, string, colour);
    default:
        return pdf_set_err(pdf, -EINVAL, "Invalid barcode code %d", code);
    }
}
