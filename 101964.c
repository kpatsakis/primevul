AffineTransform SVGElement::CalculateTransform(
    ApplyMotionTransform apply_motion_transform) const {
  const ComputedStyle* style =
      GetLayoutObject() ? GetLayoutObject()->Style() : nullptr;

  AffineTransform matrix;
  if (style && style->HasTransform()) {
    FloatRect reference_box = ComputeTransformReferenceBox(*this);
    if (TransformUsesBoxSize(*style))
      UseCounter::Count(GetDocument(), WebFeature::kTransformUsesBoxSizeOnSVG);

    float zoom = style->EffectiveZoom();
    TransformationMatrix transform;
    if (zoom != 1)
      reference_box.Scale(zoom);
    style->ApplyTransform(
        transform, reference_box, ComputedStyle::kIncludeTransformOrigin,
        ComputedStyle::kIncludeMotionPath,
        ComputedStyle::kIncludeIndependentTransformProperties);
    if (zoom != 1)
      transform.Zoom(1 / zoom);
    matrix = transform.ToAffineTransform();
  }

  if (apply_motion_transform == kIncludeMotionTransform && HasSVGRareData())
    matrix.PreMultiply(*SvgRareData()->AnimateMotionTransform());

  return matrix;
}
