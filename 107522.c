int pdf_add_line(struct pdf_doc *pdf, struct pdf_object *page,
                 int x1, int y1, int x2, int y2, int width, uint32_t colour)
{
    int ret;
    struct dstr str = {0, 0, 0};

    dstr_append(&str, "BT\r\n");
    dstr_printf(&str, "%d w\r\n", width);
    dstr_printf(&str, "%d %d m\r\n", x1, y1);
    dstr_printf(&str, "/DeviceRGB CS\r\n");
    dstr_printf(&str, "%f %f %f RG\r\n",
                PDF_RGB_R(colour), PDF_RGB_G(colour), PDF_RGB_B(colour));
    dstr_printf(&str, "%d %d l S\r\n", x2, y2);
    dstr_append(&str, "ET");

    ret = pdf_add_stream(pdf, page, str.data);
    dstr_free(&str);

    return ret;
}
