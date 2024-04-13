bool AccessibilityUIElement::isChecked() const
{
    return checkElementState(m_element, ATK_STATE_CHECKED);
}
