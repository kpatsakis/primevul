static void pdf_object_destroy(struct pdf_object *object)
{
    switch (object->type) {
    case OBJ_stream:
    case OBJ_image:
        free(object->stream.text);
        break;
    case OBJ_page:
        flexarray_clear(&object->page.children);
        break;
    case OBJ_bookmark:
        flexarray_clear(&object->bookmark.children);
        break;
    }
    free(object);
}
