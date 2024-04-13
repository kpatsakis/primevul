static int pdf_add_image(struct pdf_doc *pdf, struct pdf_object *page,
                         struct pdf_object *image, int x, int y, int width,
                         int height)
{
    int ret;
    struct dstr str = {0, 0, 0};

    dstr_append(&str, "q ");
    dstr_printf(&str, "%d 0 0 %d %d %d cm ", width, height, x, y);
    dstr_printf(&str, "/Image%d Do ", image->index);
    dstr_append(&str, "Q");

    ret = pdf_add_stream(pdf, page, str.data);
    dstr_free(&str);
    return ret;
}
