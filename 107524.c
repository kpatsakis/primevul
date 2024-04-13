int pdf_add_ppm(struct pdf_doc *pdf, struct pdf_object *page,
                int x, int y, int display_width, int display_height,
                const char *ppm_file)
{
    struct pdf_object *obj;
    uint8_t *data;
    FILE *fp;
    char line[1024];
    unsigned width, height, size;

    /* Load the PPM file */
    fp = fopen(ppm_file, "rb");
    if (!fp)
        return pdf_set_err(pdf, -errno, "Unable to open '%s'", ppm_file);
    if (!fgets(line, sizeof(line) - 1, fp)) {
        fclose(fp);
        return pdf_set_err(pdf, -EINVAL, "Invalid PPM file");
    }

    /* We only support binary ppms */
    if (strncmp(line, "P6", 2) != 0) {
        fclose(fp);
        return pdf_set_err(pdf, -EINVAL, "Only binary PPM files supported");
    }

    /* Find the width line */
    do {
        if (!fgets(line, sizeof(line) - 1, fp)) {
            fclose(fp);
            return pdf_set_err(pdf, -EINVAL, "Unable to find PPM size");
        }
        if (line[0] == '#')
            continue;

        if (sscanf(line, "%u %u\n", &width, &height) != 2) {
            fclose(fp);
            return pdf_set_err(pdf, -EINVAL, "Unable to find PPM size");
        }
        break;
    } while (1);

    /* Skip over the byte-size line */
    if (!fgets(line, sizeof(line) - 1, fp)) {
        fclose(fp);
        return pdf_set_err(pdf, -EINVAL, "No byte-size line in PPM file");
    }

    if (width > INT_MAX || height > INT_MAX) {
        fclose(fp);
        return pdf_set_err(pdf, -EINVAL, "Invalid width/height in PPM file: %ux%u", width, height);
    }

    size = width * height * 3;
    data = malloc(size);
    if (!data) {
        fclose(fp);
        return pdf_set_err(pdf, -ENOMEM, "Unable to allocate memory for RGB data");
    }
    if (fread(data, 1, size, fp) != size) {
        free(data);
        fclose(fp);
        return pdf_set_err(pdf, -EINVAL, "Insufficient RGB data available");

    }
    fclose(fp);
    obj = pdf_add_raw_rgb24(pdf, data, width, height);
    free(data);
    if (!obj)
        return pdf->errval;

    return pdf_add_image(pdf, page, obj, x, y, display_width, display_height);
}
