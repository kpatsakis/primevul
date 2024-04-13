bool RenderMenuList::itemIsSeparator(unsigned listIndex) const
{
    const Vector<HTMLElement*>& listItems = toHTMLSelectElement(node())->listItems();
    return listIndex < listItems.size() && listItems[listIndex]->hasTagName(hrTag);
}
