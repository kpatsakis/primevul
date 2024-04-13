bool GraphicsContext3D::ImageExtractor::extractImage(bool premultiplyAlpha, bool ignoreGammaAndColorProfile)
{
    UNUSED_PARAM(ignoreGammaAndColorProfile);
    if (!m_image)
        return false;

    if (m_image->data())
        m_qtImage = QImage::fromData(reinterpret_cast<const uchar*>(m_image->data()->data()), m_image->data()->size());
    else {
        QPixmap* nativePixmap = m_image->nativeImageForCurrentFrame();
        if (!nativePixmap)
            return false;

        m_qtImage = *nativePixmap->handle()->buffer();
    }

    m_alphaOp = AlphaDoNothing;
    switch (m_qtImage.format()) {
    case QImage::Format_RGB32:
        break;
    case QImage::Format_ARGB32:
        if (premultiplyAlpha)
            m_alphaOp = AlphaDoPremultiply;
        break;
    case QImage::Format_ARGB32_Premultiplied:
        if (!premultiplyAlpha)
            m_alphaOp = AlphaDoUnmultiply;
        break;
    default:
        m_qtImage = m_qtImage.convertToFormat(premultiplyAlpha ? QImage::Format_ARGB32_Premultiplied : QImage::Format_ARGB32);
        break;
    }

    m_imageWidth = m_image->width();
    m_imageHeight = m_image->height();
    if (!m_imageWidth || !m_imageHeight)
        return false;
    m_imagePixelData = m_qtImage.constBits();
    m_imageSourceFormat = SourceFormatBGRA8;
    m_imageSourceUnpackAlignment = 0;

    return true;
}
