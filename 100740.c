bool XSLStyleSheet::isLoading() const
{
    for (unsigned i = 0; i < m_children.size(); ++i) {
        if (m_children.at(i)->isLoading())
            return true;
    }
    return false;
}
