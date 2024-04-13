bool HTMLAnchorElement::shouldHaveFocusAppearance() const
{
    return !m_wasFocusedByMouse || HTMLElement::supportsFocus();
}
