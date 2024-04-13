void v9fs_path_copy(V9fsPath *lhs, V9fsPath *rhs)
{
    v9fs_path_free(lhs);
    lhs->data = g_malloc(rhs->size);
    memcpy(lhs->data, rhs->data, rhs->size);
    lhs->size = rhs->size;
}
