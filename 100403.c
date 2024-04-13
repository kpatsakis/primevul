BackgroundBleedAvoidance RenderBox::determineBackgroundBleedAvoidance(GraphicsContext* context) const
{
    if (context->paintingDisabled())
        return BackgroundBleedNone;

    const RenderStyle* style = this->style();

    if (!style->hasBackground() || !style->hasBorder() || !style->hasBorderRadius() || borderImageIsLoadedAndCanBeRendered())
        return BackgroundBleedNone;

    AffineTransform ctm = context->getCTM();
    FloatSize contextScaling(static_cast<float>(ctm.xScale()), static_cast<float>(ctm.yScale()));
    if (borderObscuresBackgroundEdge(contextScaling))
        return BackgroundBleedShrinkBackground;
    

    return BackgroundBleedUseTransparencyLayer;
}
