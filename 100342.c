void RenderMenuList::didSetSelectedIndex(int listIndex)
{
    didUpdateActiveOption(toHTMLSelectElement(node())->listToOptionIndex(listIndex));
}
