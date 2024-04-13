XYToWindow(SpritePtr pSprite, int x, int y)
{
    ScreenPtr pScreen = RootWindow(pSprite)->drawable.pScreen;

    return (*pScreen->XYToWindow)(pScreen, pSprite, x, y);
}
