static pdf_object *pdf_add_raw_rgb24(struct pdf_doc *pdf,
                                     uint8_t *data, int width, int height)
{
    struct pdf_object *obj;
    char line[1024];
    int len;
    uint8_t *final_data;
    const char *endstream = ">\r\nendstream\r\n";
    int i;

    sprintf(line,
            "<<\r\n/Type /XObject\r\n/Name /Image%d\r\n/Subtype /Image\r\n"
            "/ColorSpace /DeviceRGB\r\n/Height %d\r\n/Width %d\r\n"
            "/BitsPerComponent 8\r\n/Filter /ASCIIHexDecode\r\n"
            "/Length %d\r\n>>stream\r\n",
            flexarray_size(&pdf->objects), height, width, width * height * 3 * 2 + 1);

    len = strlen(line) + width * height * 3 * 2 + strlen(endstream) + 1;
    final_data = malloc(len);
    if (!final_data) {
        pdf_set_err(pdf, -ENOMEM, "Unable to allocate %d bytes memory for image",
                    len);
        return NULL;
    }
    strcpy((char *)final_data, line);
    uint8_t *pos = &final_data[strlen(line)];
    for (i = 0; i < width * height * 3; i++) {
        *pos++ = "0123456789ABCDEF"[(data[i] >> 4) & 0xf];
        *pos++ = "0123456789ABCDEF"[data[i] & 0xf];
    }
    strcpy((char *)pos, endstream);
    pos += strlen(endstream);

    obj = pdf_add_object(pdf, OBJ_image);
    if (!obj) {
        free(final_data);
        return NULL;
    }
    obj->stream.text = (char *)final_data;
    obj->stream.len = pos - final_data;

    return obj;
}
