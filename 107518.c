int pdf_add_circle(struct pdf_doc *pdf, struct pdf_object *page,
                   int x, int y, int radius, int width, uint32_t colour, bool filled)
{
    int ret;
    struct dstr str = {0, 0, 0};

    dstr_append(&str, "BT ");
    if (filled)
        dstr_printf(&str, "%f %f %f rg ",
                    PDF_RGB_R(colour), PDF_RGB_G(colour), PDF_RGB_B(colour));
    else
        dstr_printf(&str, "%f %f %f RG ",
                    PDF_RGB_R(colour), PDF_RGB_G(colour), PDF_RGB_B(colour));
    dstr_printf(&str, "%d w ", width);
    /* This is a bit of a rough approximation of a circle based on bezier curves.
     * It's not exact
     */
    dstr_printf(&str, "%d %d m ", x + radius, y);
    dstr_printf(&str, "%d %d %d %d v ", x + radius, y + radius, x, y + radius);
    dstr_printf(&str, "%d %d %d %d v ", x - radius, y + radius, x - radius, y);
    dstr_printf(&str, "%d %d %d %d v ", x - radius, y - radius, x, y - radius);
    dstr_printf(&str, "%d %d %d %d v ", x + radius, y - radius, x + radius, y);
    if (filled)
        dstr_append(&str, "f ");
    else
        dstr_append(&str, "S ");
    dstr_append(&str, "ET");
    ret = pdf_add_stream(pdf, page, str.data);
    dstr_free(&str);

    return ret;
}
