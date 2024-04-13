static int pdf_text_pixel_width(const char *text, int text_len, int size,
                                const uint16_t *widths)
{
    int i;
    int len = 0;
    if (text_len < 0)
        text_len = strlen(text);

    for (i = 0; i < text_len; i++)
        len += widths[(uint8_t)text[i]];

    /* Our widths arrays are for 14pt fonts */
    return len * size / (14 * 72);
}
