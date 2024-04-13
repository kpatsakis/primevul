struct pdf_doc *pdf_create(int width, int height, struct pdf_info *info)
{
    struct pdf_doc *pdf;
    struct pdf_object *obj;

    pdf = calloc(1, sizeof(struct pdf_doc));
    pdf->width = width;
    pdf->height = height;

    /* We don't want to use ID 0 */
    pdf_add_object(pdf, OBJ_none);

    /* Create the 'info' object */
    obj = pdf_add_object(pdf, OBJ_info);
    if (info)
        obj->info = *info;
    /* FIXME: Should be quoting PDF strings? */
    if (!obj->info.date[0]) {
        time_t now = time(NULL);
        struct tm tm;
#ifdef _WIN32
        struct tm *tmp;
        tmp = localtime(&now);
        tm = *tmp;
#else
        localtime_r(&now, &tm);
#endif
        strftime(obj->info.date, sizeof(obj->info.date),
                 "%Y%m%d%H%M%SZ", &tm);
    }
    if (!obj->info.creator[0])
        strcpy(obj->info.creator, "pdfgen");
    if (!obj->info.producer[0])
        strcpy(obj->info.producer, "pdfgen");
    if (!obj->info.title[0])
        strcpy(obj->info.title, "pdfgen");
    if (!obj->info.author[0])
        strcpy(obj->info.author, "pdfgen");
    if (!obj->info.subject[0])
        strcpy(obj->info.subject, "pdfgen");

    pdf_add_object(pdf, OBJ_pages);
    pdf_add_object(pdf, OBJ_catalog);

    pdf_set_font(pdf, "Times-Roman");

    return pdf;
}
