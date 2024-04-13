void LayerWebKitThread::setContents(Image* contents)
{
    if (m_contents == contents) {
        if (contents)
            setNeedsDisplay();
        return;
    }
    m_contents = contents;
    setNeedsTexture(m_isDrawable && (this->contents() || drawsContent() || pluginView()));

    if (m_contents)
        setNeedsDisplay();
    else
        setNeedsCommit();

    m_contentsResolutionIndependent = static_cast<bool>(m_contents);
}
