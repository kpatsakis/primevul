int pdf_get_font_text_width(struct pdf_doc *pdf, const char *font_name,
                            const char *text, int size)
{
    const uint16_t *widths = find_font_widths(font_name);

    if (!widths)
        return pdf_set_err(pdf, -EINVAL, "Unable to determine width for font '%s'",
                           pdf->current_font->font.name);
    return pdf_text_pixel_width(text, -1, size, widths);
}
