static void dstr_free(struct dstr *str)
{
    free(str->data);
}
