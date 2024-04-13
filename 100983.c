void LayerWebKitThread::setDrawable(bool isDrawable)
{
    if (m_isDrawable == isDrawable)
        return;

    m_isDrawable = isDrawable;

    setNeedsTexture(m_isDrawable && (drawsContent() || contents() || pluginView() || mediaPlayer()));
    setNeedsCommit();
}
