static inline bool TransformUsesBoxSize(const ComputedStyle& style) {
  if ((style.TransformOriginX().IsPercent() ||
       style.TransformOriginY().IsPercent()) &&
      style.RequireTransformOrigin(ComputedStyle::kIncludeTransformOrigin,
                                   ComputedStyle::kExcludeMotionPath))
    return true;
  if (style.Transform().DependsOnBoxSize())
    return true;
  if (style.Translate() && style.Translate()->DependsOnBoxSize())
    return true;
  if (style.HasOffset())
    return true;
  return false;
}
