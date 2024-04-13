static const uint16_t *find_font_widths(const char *font_name)
{
    if (strcmp(font_name, "Helvetica") == 0)
        return helvetica_widths;
    if (strcmp(font_name, "Helvetica-Bold") == 0)
        return helvetica_bold_widths;
    if (strcmp(font_name, "Helvetica-BoldOblique") == 0)
        return helvetica_bold_oblique_widths;
    if (strcmp(font_name, "Helvetica-Oblique") == 0)
        return helvetica_oblique_widths;
    if (strcmp(font_name, "Courier") == 0 ||
            strcmp(font_name, "Courier-Bold") == 0 ||
            strcmp(font_name, "Courier-BoldOblique") == 0 ||
            strcmp(font_name, "Courier-Oblique") == 0)
        return courier_widths;
    if (strcmp(font_name, "Times-Roman") == 0)
        return times_widths;
    if (strcmp(font_name, "Times-Bold") == 0)
        return times_bold_widths;
    if (strcmp(font_name, "Times-Italic") == 0)
        return times_italic_widths;
    if (strcmp(font_name, "Times-BoldItalic") == 0)
        return times_bold_italic_widths;
    if (strcmp(font_name, "Symbol") == 0)
        return symbol_widths;
    if (strcmp(font_name, "ZapfDingbats") == 0)
        return zapfdingbats_widths;

    return NULL;
}
