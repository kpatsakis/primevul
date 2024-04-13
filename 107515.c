static int pdf_add_barcode_128a(struct pdf_doc *pdf, struct pdf_object *page,
                                int x, int y, int width, int height,
                                const char *string, uint32_t colour)
{
    const char *s;
    int len = strlen(string) + 3;
    int char_width = width / len;
    int checksum, i;

    for (s = string; *s; s++)
        if (find_128_encoding(*s) < 0)
            return pdf_set_err(pdf, -EINVAL, "Invalid barcode character 0x%x", *s);

    x = pdf_barcode_128a_ch(pdf, page, x, y, char_width, height, colour, 104,
                            6);
    checksum = 104;

    for (i = 1, s = string; *s; s++, i++) {
        int index = find_128_encoding(*s);
        x = pdf_barcode_128a_ch(pdf, page, x, y, char_width, height, colour, index,
                                6);
        checksum += index * i;
    }
    x = pdf_barcode_128a_ch(pdf, page, x, y, char_width, height, colour,
                            checksum % 103, 6);
    pdf_barcode_128a_ch(pdf, page, x, y, char_width, height, colour, 106,
                        7);
    return 0;
}
