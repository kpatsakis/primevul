void LayerTilerChromium::update(TilePaintInterface& painter, const IntRect& contentRect)
{
    if (m_skipsDraw)
        return;

    IntRect layerRect = contentRectToLayerRect(contentRect);
    invalidateTiles(m_lastUpdateLayerRect, layerRect);
    m_lastUpdateLayerRect = layerRect;

    growLayerToContain(contentRect);

    IntRect dirtyLayerRect;
    int left, top, right, bottom;
    contentRectToTileIndices(contentRect, left, top, right, bottom);
    for (int j = top; j <= bottom; ++j) {
        for (int i = left; i <= right; ++i) {
            Tile* tile = m_tiles[tileIndex(i, j)].get();
            if (!tile)
                tile = createTile(i, j);
            if (!tile->texture()->isValid(m_tileSize, GraphicsContext3D::RGBA))
                tile->m_dirtyLayerRect = tileLayerRect(i, j);
            dirtyLayerRect.unite(tile->m_dirtyLayerRect);
        }
    }

    if (dirtyLayerRect.isEmpty())
        return;

    const IntRect paintRect = layerRectToContentRect(dirtyLayerRect);
    GraphicsContext3D* context = layerRendererContext();
#if PLATFORM(SKIA)
    OwnPtr<skia::PlatformCanvas> canvas(new skia::PlatformCanvas(paintRect.width(), paintRect.height(), false));
    OwnPtr<PlatformContextSkia> skiaContext(new PlatformContextSkia(canvas.get()));
    OwnPtr<GraphicsContext> graphicsContext(new GraphicsContext(reinterpret_cast<PlatformGraphicsContext*>(skiaContext.get())));

    canvas->translate(static_cast<SkScalar>(-paintRect.x()), static_cast<SkScalar>(-paintRect.y()));

    painter.paint(*graphicsContext, paintRect);

    const SkBitmap& bitmap = canvas->getDevice()->accessBitmap(false);
    ASSERT(bitmap.width() == paintRect.width() && bitmap.height() == paintRect.height());
    if (bitmap.width() != paintRect.width() || bitmap.height() != paintRect.height())
        CRASH();
    uint8_t* paintPixels = static_cast<uint8_t*>(bitmap.getPixels());
    if (!paintPixels)
        CRASH();
#elif PLATFORM(CG)
    Vector<uint8_t> canvasPixels;
    int rowBytes = 4 * paintRect.width();
    canvasPixels.resize(rowBytes * paintRect.height());
    memset(canvasPixels.data(), 0, canvasPixels.size());
    RetainPtr<CGColorSpaceRef> colorSpace(AdoptCF, CGColorSpaceCreateDeviceRGB());
    RetainPtr<CGContextRef> m_cgContext;
    m_cgContext.adoptCF(CGBitmapContextCreate(canvasPixels.data(),
                                                       paintRect.width(), paintRect.height(), 8, rowBytes,
                                                       colorSpace.get(),
                                                       kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host));
    CGContextTranslateCTM(m_cgContext.get(), 0, paintRect.height());
    CGContextScaleCTM(m_cgContext.get(), 1, -1);
    OwnPtr<GraphicsContext> m_graphicsContext(new GraphicsContext(m_cgContext.get()));

    CGContextTranslateCTM(m_cgContext.get(), -paintRect.x(), -paintRect.y());
    painter.paint(*m_graphicsContext, paintRect);

    ASSERT(static_cast<int>(CGBitmapContextGetWidth(m_cgContext.get())) == paintRect.width() && static_cast<int>(CGBitmapContextGetHeight(m_cgContext.get())) == paintRect.height());
    uint8_t* paintPixels = static_cast<uint8_t*>(canvasPixels.data());
#else
#error "Need to implement for your platform."
#endif

    if (!m_tiles.size())
        return;

    for (int j = top; j <= bottom; ++j) {
        for (int i = left; i <= right; ++i) {
            Tile* tile = m_tiles[tileIndex(i, j)].get();
            if (!tile)
                CRASH();
            if (!tile->dirty())
                continue;

            IntRect sourceRect = tileContentRect(i, j);
            const IntPoint anchor = sourceRect.location();
            sourceRect.intersect(layerRectToContentRect(tile->m_dirtyLayerRect));
            if (sourceRect.isEmpty())
                continue;

            if (!tile->texture()->reserve(m_tileSize, GraphicsContext3D::RGBA)) {
                m_skipsDraw = true;
                reset();
                return;
            }

            IntRect destRect(IntPoint(sourceRect.x() - anchor.x(), sourceRect.y() - anchor.y()), sourceRect.size());
            if (destRect.x() < 0)
                CRASH();
            if (destRect.y() < 0)
                CRASH();

            IntPoint paintOffset(sourceRect.x() - paintRect.x(), sourceRect.y() - paintRect.y());
            if (paintOffset.x() < 0)
                CRASH();
            if (paintOffset.y() < 0)
                CRASH();
            if (paintOffset.x() + destRect.width() > paintRect.width())
                CRASH();
            if (paintOffset.y() + destRect.height() > paintRect.height())
                CRASH();

            uint8_t* pixelSource;
            if (paintRect.width() == sourceRect.width() && !paintOffset.x())
                pixelSource = &paintPixels[4 * paintOffset.y() * paintRect.width()];
            else {
                for (int row = 0; row < destRect.height(); ++row)
                    memcpy(&m_tilePixels[destRect.width() * 4 * row],
                           &paintPixels[4 * (paintOffset.x() + (paintOffset.y() + row) * paintRect.width())],
                           destRect.width() * 4);

                pixelSource = &m_tilePixels[0];
            }

            tile->texture()->bindTexture();
            GLC(context, context->texSubImage2D(GraphicsContext3D::TEXTURE_2D, 0, destRect.x(), destRect.y(), destRect.width(), destRect.height(), GraphicsContext3D::RGBA, GraphicsContext3D::UNSIGNED_BYTE, pixelSource));

            tile->clearDirty();
        }
    }
}
