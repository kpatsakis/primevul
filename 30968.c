IsParent(WindowPtr a, WindowPtr b)
{
    for (b = b->parent; b; b = b->parent)
        if (b == a)
            return TRUE;
    return FALSE;
}
