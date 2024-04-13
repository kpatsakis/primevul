static int pdf_add_barcode_39(struct pdf_doc *pdf, struct pdf_object *page,
                              int x, int y, int width, int height,
                              const char *string, uint32_t colour)
{
    int len = strlen(string);
    int char_width = width / (len + 2);

    x = pdf_barcode_39_ch(pdf, page, x, y, char_width, height, colour, '*');
    if (x < 0)
        return x;

    while (string && *string) {
        x = pdf_barcode_39_ch(pdf, page, x, y, char_width, height, colour, *string);
        if (x < 0)
            return x;
        string++;
    };

    x = pdf_barcode_39_ch(pdf, page, x, y, char_width, height, colour, '*');
    if (x < 0)
        return x;

    return 0;
}
