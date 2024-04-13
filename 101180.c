void Document::updateFocusAppearanceSoon(SelectionBehaviorOnFocus selectionbehavioronfocus)
{
    m_updateFocusAppearanceSelectionBahavior = selectionbehavioronfocus;
    if (!m_updateFocusAppearanceTimer.isActive())
        m_updateFocusAppearanceTimer.startOneShot(0, BLINK_FROM_HERE);
}
