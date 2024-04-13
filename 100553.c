JSRetainPtr<JSStringRef> AccessibilityUIElement::rowIndexRange()
{
    return indexRangeInTable(m_element, true);
}
