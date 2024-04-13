    void testAnimatedWithoutClear()
    {
        m_testSurface->initializeCurrentFrame();
        m_fakeImageBufferClient->fakeDraw();
        m_testSurface->getPicture();
        EXPECT_EQ(1, m_fakeImageBufferClient->frameCount());
        EXPECT_EQ(0, m_surfaceFactory->createSurfaceCount());
        expectDisplayListEnabled(true); // first frame has an implicit clear
        m_fakeImageBufferClient->fakeDraw();
        m_testSurface->getPicture();
        EXPECT_EQ(2, m_fakeImageBufferClient->frameCount());
        expectDisplayListEnabled(false);
    }
