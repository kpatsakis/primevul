void simulatePageScale(WebViewImpl* webViewImpl, float& scale)
{
    WebCore::IntSize scrollDelta = webViewImpl->fakePageScaleAnimationTargetPositionForTesting() - webViewImpl->mainFrameImpl()->frameView()->scrollPosition();
    float scaleDelta = webViewImpl->fakePageScaleAnimationPageScaleForTesting() / webViewImpl->pageScaleFactor();
    webViewImpl->applyScrollAndScale(scrollDelta, scaleDelta);
    scale = webViewImpl->pageScaleFactor();
}
