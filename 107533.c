static int pdf_barcode_128a_ch(struct pdf_doc *pdf, struct pdf_object *page,
                               int x, int y, int width, int height,
                               uint32_t colour, int index, int code_len)
{
    uint32_t code = code_128a_encoding[index].code;
    int i;
    int line_width = width / 11;

    for (i = 0; i < code_len; i++) {
        uint8_t shift = (code_len - 1 - i) * 4;
        uint8_t mask = (code >> shift) & 0xf;

        if (!(i % 2)) {
            int j;
            for (j = 0; j < mask; j++) {
                pdf_add_line(pdf, page, x, y, x, y + height, line_width, colour);
                x += line_width;
            }
        } else
            x += line_width * mask;
    }
    return x;
}
