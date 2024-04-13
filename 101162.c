Element* Document::elementFromPoint(int x, int y) const
{
    if (!layoutView())
        return 0;

    return TreeScope::elementFromPoint(x, y);
}
