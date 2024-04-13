DEFINE_TRACE(SVGFEColorMatrixElement)
{
    visitor->trace(m_values);
    visitor->trace(m_in1);
    visitor->trace(m_type);
    SVGFilterPrimitiveStandardAttributes::trace(visitor);
}
