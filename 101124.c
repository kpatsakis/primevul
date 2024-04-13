void RenderSVGImage::paintForeground(PaintInfo& paintInfo)
{
    RefPtr<Image> image = m_imageResource->image();
    FloatRect destRect = m_objectBoundingBox;
    FloatRect srcRect(0, 0, image->width(), image->height());

    SVGImageElement* imageElement = toSVGImageElement(element());
    imageElement->preserveAspectRatioCurrentValue().transformRect(destRect, srcRect);

    bool useLowQualityScaling = false;
    if (style()->svgStyle()->bufferedRendering() != BR_STATIC)
        useLowQualityScaling = ImageQualityController::imageQualityController()->shouldPaintAtLowQuality(paintInfo.context, this, image.get(), image.get(), LayoutSize(destRect.size()));

    paintInfo.context->drawImage(image.get(), destRect, srcRect, CompositeSourceOver, DoNotRespectImageOrientation, useLowQualityScaling);
}
