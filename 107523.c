static struct pdf_object *pdf_add_object(struct pdf_doc *pdf, int type)
{
    struct pdf_object *obj;

    obj = calloc(1, sizeof(struct pdf_object));
    if (!obj) {
        pdf_set_err(pdf, -errno, "Unable to allocate object %d: %s",
                    flexarray_size(&pdf->objects) + 1, strerror(errno));
        return NULL;
    }

    obj->type = type;

    if (pdf_append_object(pdf, obj) < 0) {
        free(obj);
        return NULL;
    }

    return obj;
}
