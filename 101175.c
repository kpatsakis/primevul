void Document::setViewportDescription(const ViewportDescription& viewportDescription)
{
    if (viewportDescription == m_viewportDescription)
        return;

    if (!viewportDescription.isSpecifiedByAuthor())
        m_viewportDefaultMinWidth = viewportDescription.minWidth;

    if (viewportDescription.isLegacyViewportType()) {
        m_legacyViewportDescription = viewportDescription;

        if (m_viewportDescription.type == ViewportDescription::AuthorStyleSheet)
            return;
        m_viewportDescription = viewportDescription;
    } else {
        if (!shouldOverrideLegacyDescription(viewportDescription.type))
            m_viewportDescription = m_legacyViewportDescription;
        else
            m_viewportDescription = viewportDescription;
    }

    updateViewportDescription();
}
