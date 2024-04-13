int pdf_add_text(struct pdf_doc *pdf, struct pdf_object *page,
                 const char *text, int size, int xoff, int yoff,
                 uint32_t colour)
{
    int i, ret;
    int len = text ? strlen(text) : 0;
    struct dstr str = {0, 0, 0};

    /* Don't bother adding empty/null strings */
    if (!len)
        return 0;

    dstr_append(&str, "BT ");
    dstr_printf(&str, "%d %d TD ", xoff, yoff);
    dstr_printf(&str, "/F%d %d Tf ",
                pdf->current_font->font.index, size);
    dstr_printf(&str, "%f %f %f rg ",
                PDF_RGB_R(colour), PDF_RGB_G(colour), PDF_RGB_B(colour));
    dstr_append(&str, "(");

    /* Escape magic characters properly */
    for (i = 0; i < len; ) {
        uint32_t code;
        int code_len;
        code_len = utf8_to_utf32(&text[i], len - i, &code);
        if (code_len < 0) {
            dstr_free(&str);
            return pdf_set_err(pdf, -EINVAL, "Invalid UTF-8 encoding");
        }

        if (code > 255) {
            /* We support *some* minimal UTF-8 characters */
            char buf[5] = {0};
            switch (code) {
            case 0x160:
                buf[0] = (char)0x8a;
                break;
            case 0x161:
                buf[0] = (char)0x9a;
                break;
            case 0x17d:
                buf[0] = (char)0x8e;
                break;
            case 0x17e:
                buf[0] = (char)0x9e;
                break;
            case 0x20ac:
                strcpy(buf, "\\200");
                break;
            default:
                dstr_free(&str);
                return pdf_set_err(pdf, -EINVAL, "Unsupported UTF-8 character: 0x%x 0o%o", code, code);
            }
            dstr_append(&str, buf);
        } else if (strchr("()\\", code)) {
            char buf[3];
            /* Escape some characters */
            buf[0] = '\\';
            buf[1] = code;
            buf[2] = '\0';
            dstr_append(&str, buf);
        } else if (strrchr("\n\r\t\b\f", code)) {
            /* Skip over these characters */
            ;
        } else {
            char buf[2];
            buf[0] = code;
            buf[1] = '\0';
            dstr_append(&str, buf);
        }

        i += code_len;
    }
    dstr_append(&str, ") Tj ");
    dstr_append(&str, "ET");

    ret = pdf_add_stream(pdf, page, str.data);
    dstr_free(&str);
    return ret;
}
