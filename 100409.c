void RenderBox::paintFillLayer(const PaintInfo& paintInfo, const Color& c, const FillLayer* fillLayer, const LayoutRect& rect,
    BackgroundBleedAvoidance bleedAvoidance, CompositeOperator op, RenderObject* backgroundObject)
{
    paintFillLayerExtended(paintInfo, c, fillLayer, rect, bleedAvoidance, 0, IntSize(), op, backgroundObject);
}
