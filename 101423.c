    virtual PassOwnPtr<ImageBufferSurface> createSurface(const IntSize& size, OpacityMode opacityMode)
    {
        m_createSurfaceCount++;
        return adoptPtr(new UnacceleratedImageBufferSurface(size, opacityMode));
    }
