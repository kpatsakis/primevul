int pdf_add_rectangle(struct pdf_doc *pdf, struct pdf_object *page,
                      int x, int y, int width, int height, int border_width,
                      uint32_t colour)
{
    int ret;
    struct dstr str = {0, 0, 0};

    dstr_append(&str, "BT ");
    dstr_printf(&str, "%f %f %f RG ",
                PDF_RGB_R(colour), PDF_RGB_G(colour), PDF_RGB_B(colour));
    dstr_printf(&str, "%d w ", border_width);
    dstr_printf(&str, "%d %d %d %d re S ", x, y, width, height);
    dstr_append(&str, "ET");

    ret = pdf_add_stream(pdf, page, str.data);
    dstr_free(&str);

    return ret;
}
