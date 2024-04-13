int pdf_add_text_wrap(struct pdf_doc *pdf, struct pdf_object *page,
                      const char *text, int size, int xoff, int yoff,
                      uint32_t colour, int wrap_width)
{
    /* Move through the text string, stopping at word boundaries,
     * trying to find the longest text string we can fit in the given width
     */
    const char *start = text;
    const char *last_best = text;
    const char *end = text;
    char line[512];
    const uint16_t *widths;
    int orig_yoff = yoff;

    widths = find_font_widths(pdf->current_font->font.name);
    if (!widths)
        return pdf_set_err(pdf, -EINVAL, "Unable to determine width for font '%s'",
                           pdf->current_font->font.name);

    while (start && *start) {
        const char *new_end = find_word_break(end + 1);
        int line_width;
        int output = 0;

        end = new_end;

        line_width = pdf_text_pixel_width(start, end - start, size, widths);

        if (line_width >= wrap_width) {
            if (last_best == start) {
                /* There is a single word that is too long for the line */
                int i;
                /* Find the best character to chop it at */
                for (i = end - start - 1; i > 0; i--)
                    if (pdf_text_pixel_width(start, i, size, widths) < wrap_width)
                        break;

                end = start + i;
            } else
                end = last_best;
            output = 1;
        }
        if (*end == '\0')
            output = 1;

        if (*end == '\n' || *end == '\r')
            output = 1;

        if (output) {
            int len = end - start;
            strncpy(line, start, len);
            line[len] = '\0';
            pdf_add_text(pdf, page, line, size, xoff, yoff, colour);

            if (*end == ' ')
                end++;

            start = last_best = end;
            yoff -= size;
        } else
            last_best = end;
    }

    return orig_yoff - yoff;
}
