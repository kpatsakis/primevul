static int dstr_ensure(struct dstr *str, int len)
{
    if (str->alloc_len < len) {
        int new_len = len + 4096;
        char *new_data = realloc(str->data, new_len);
        if (!new_data)
            return -ENOMEM;
        str->data = new_data;
        str->alloc_len = new_len;
    }
    return 0;
}
