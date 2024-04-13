static pdf_object *pdf_add_raw_jpeg(struct pdf_doc *pdf,
                                    const char *jpeg_file)
{
    struct stat buf;
    off_t len;
    char *final_data;
    uint8_t *jpeg_data;
    int written = 0;
    FILE *fp;
    struct pdf_object *obj;
    int width, height;

    if (stat(jpeg_file, &buf) < 0) {
        pdf_set_err(pdf, -errno, "Unable to access %s: %s", jpeg_file,
                    strerror(errno));
        return NULL;
    }

    len = buf.st_size;

    if ((fp = fopen(jpeg_file, "rb")) == NULL) {
        pdf_set_err(pdf, -errno, "Unable to open %s: %s", jpeg_file,
                    strerror(errno));
        return NULL;
    }

    jpeg_data = malloc(len);
    if (!jpeg_data) {
        pdf_set_err(pdf, -errno, "Unable to allocate: %zd", len);
        fclose(fp);
        return NULL;
    }

    if (fread(jpeg_data, len, 1, fp) != 1) {
        pdf_set_err(pdf, -errno, "Unable to read full jpeg data");
        free(jpeg_data);
        fclose(fp);
        return NULL;
    }
    fclose(fp);

    if (jpeg_size(jpeg_data, len, &width, &height) < 0) {
        free(jpeg_data);
        pdf_set_err(pdf, -EINVAL, "Unable to determine jpeg width/height from %s",
                    jpeg_file);
        return NULL;
    }

    final_data = malloc(len + 1024);
    if (!final_data) {
        pdf_set_err(pdf, -errno, "Unable to allocate jpeg data %zd", len + 1024);
        free(jpeg_data);
        return NULL;
    }

    written = sprintf(final_data,
                      "<<\r\n/Type /XObject\r\n/Name /Image%d\r\n"
                      "/Subtype /Image\r\n/ColorSpace /DeviceRGB\r\n"
                      "/Width %d\r\n/Height %d\r\n"
                      "/BitsPerComponent 8\r\n/Filter /DCTDecode\r\n"
                      "/Length %d\r\n>>stream\r\n",
                      flexarray_size(&pdf->objects), width, height, (int)len);
    memcpy(&final_data[written], jpeg_data, len);
    written += len;
    written += sprintf(&final_data[written], "\r\nendstream\r\n");

    free(jpeg_data);

    obj = pdf_add_object(pdf, OBJ_image);
    if (!obj) {
        free(final_data);
        return NULL;
    }
    obj->stream.text = final_data;
    obj->stream.len = written;

    return obj;
}
