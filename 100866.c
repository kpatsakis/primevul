bool RenderLayerCompositor::supportsFixedRootBackgroundCompositing() const
{
    if (Settings* settings = m_renderView->document().settings()) {
        if (settings->acceleratedCompositingForFixedRootBackgroundEnabled())
            return true;
    }
    return false;
}
