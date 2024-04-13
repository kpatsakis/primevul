static PassRefPtr<CSSValueList> getBorderRadiusCornerValues(LengthSize radius, const RenderStyle* style, RenderView* renderView)
{
    RefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
    if (radius.width().type() == Percent)
        list->append(cssValuePool().createValue(radius.width().percent(), CSSPrimitiveValue::CSS_PERCENTAGE));
    else
        list->append(zoomAdjustedPixelValue(valueForLength(radius.width(), 0, renderView), style));
    if (radius.height().type() == Percent)
        list->append(cssValuePool().createValue(radius.height().percent(), CSSPrimitiveValue::CSS_PERCENTAGE));
    else
        list->append(zoomAdjustedPixelValue(valueForLength(radius.height(), 0, renderView), style));
    return list.release();
}
