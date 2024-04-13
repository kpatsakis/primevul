uint32_t GraphicsContext3DPrivate::copyToGraphicsSurface()
{
    if (!m_graphicsSurface)
        return 0;

    blitMultisampleFramebufferAndRestoreContext();
    m_graphicsSurface->copyFromTexture(m_context->m_texture, IntRect(0, 0, m_context->m_currentWidth, m_context->m_currentHeight));
    return m_graphicsSurface->frontBuffer();
}
