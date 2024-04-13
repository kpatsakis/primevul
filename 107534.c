static int pdf_barcode_39_ch(struct pdf_doc *pdf, struct pdf_object *page, int x, int y, int char_width, int height, uint32_t colour, char ch)
{
    int nw = char_width / 12;
    int ww = char_width / 4;
    int i;
    uint32_t code;

    if (nw <= 1 || ww <= 1)
        return pdf_set_err(pdf, -EINVAL, "Insufficient width for each character");

    for (i = 0; i < ARRAY_SIZE(code_39_encoding); i++) {
        if (code_39_encoding[i].ch == ch) {
            code = code_39_encoding[i].code;
            break;
        }
    }
    if (i == ARRAY_SIZE(code_39_encoding))
        return pdf_set_err(pdf, -EINVAL, "Invalid Code 39 character %c 0x%x", ch, ch);


    for (i = 5; i >= 0; i--) {
        int pattern = (code >> i * 4) & 0xf;
        if (pattern == 0) { // wide
            if (pdf_add_filled_rectangle(pdf, page, x, y, ww - 1, height, 0, colour) < 0)
                return pdf->errval;
            x += ww;
        }
        if (pattern == 1) { // narrow
            if (pdf_add_filled_rectangle(pdf, page, x, y, nw - 1, height, 0, colour) < 0)
                return pdf->errval;
            x += nw;
        }
        if (pattern == 2) { // space
            x += nw;
        }
    }
    return x;
}
