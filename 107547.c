static int pdf_save_object(struct pdf_doc *pdf, FILE *fp, int index)
{
    struct pdf_object *object = pdf_get_object(pdf, index);

    if (object->type == OBJ_none)
        return -ENOENT;

    object->offset = ftell(fp);

    fprintf(fp, "%d 0 obj\r\n", index);

    switch (object->type) {
    case OBJ_stream:
    case OBJ_image: {
        int len = object->stream.len ? object->stream.len :
                  strlen(object->stream.text);
        fwrite(object->stream.text, len, 1, fp);
        break;
    }
    case OBJ_info: {
        struct pdf_info *info = &object->info;

        fprintf(fp, "<<\r\n"
                "  /Creator (%s)\r\n"
                "  /Producer (%s)\r\n"
                "  /Title (%s)\r\n"
                "  /Author (%s)\r\n"
                "  /Subject (%s)\r\n"
                "  /CreationDate (D:%s)\r\n"
                ">>\r\n",
                info->creator, info->producer, info->title,
                info->author, info->subject, info->date);
        break;
    }

    case OBJ_page: {
        int i;
        struct pdf_object *font;
        struct pdf_object *pages = pdf_find_first_object(pdf, OBJ_pages);
        struct pdf_object *image = pdf_find_first_object(pdf, OBJ_image);

        fprintf(fp, "<<\r\n"
                "/Type /Page\r\n"
                "/Parent %d 0 R\r\n", pages->index);
        fprintf(fp, "/MediaBox [0 0 %d %d]\r\n",
                object->page.width, object->page.height);
        fprintf(fp, "/Resources <<\r\n");
        fprintf(fp, "  /Font <<\r\n");
        for (font = pdf_find_first_object(pdf, OBJ_font); font; font = font->next)
            fprintf(fp, "    /F%d %d 0 R\r\n",
                    font->font.index, font->index);
        fprintf(fp, "  >>\r\n");

        if (image) {
            fprintf(fp, "  /XObject <<");
            for (; image; image = image->next)
                fprintf(fp, "/Image%d %d 0 R ", image->index, image->index);
            fprintf(fp, ">>\r\n");
        }

        fprintf(fp, ">>\r\n");
        fprintf(fp, "/Contents [\r\n");
        for (i = 0; i < flexarray_size(&object->page.children); i++) {
            struct pdf_object *child = flexarray_get(&object->page.children, i);
            fprintf(fp, "%d 0 R\r\n", child->index);
        }
        fprintf(fp, "]\r\n");
        fprintf(fp, ">>\r\n");
        break;
    }

    case OBJ_bookmark: {
        struct pdf_object *parent, *other;

        parent = object->bookmark.parent;
        if (!parent)
            parent = pdf_find_first_object(pdf, OBJ_outline);
        if (!object->bookmark.page)
            break;
        fprintf(fp, "<<\r\n"
                "/A << /Type /Action\r\n"
                "      /S /GoTo\r\n"
                "      /D [%d 0 R /XYZ 0 %d null]\r\n"
                "   >>\r\n"
                "/Parent %d 0 R\r\n"
                "/Title (%s)\r\n",
                object->bookmark.page->index,
                pdf->height,
                parent->index,
                object->bookmark.name);
        int nchildren = flexarray_size(&object->bookmark.children);
        if (nchildren > 0) {
            struct pdf_object *f, *l;
            f = flexarray_get(&object->bookmark.children, 0);
            l = flexarray_get(&object->bookmark.children, nchildren - 1);
            fprintf(fp, "/First %d 0 R\r\n", f->index);
            fprintf(fp, "/Last %d 0 R\r\n", l->index);
        }
        for (other = object->prev;
                other && other->bookmark.parent != object->bookmark.parent;
                other = other->prev)
            ;
        if (other)
            fprintf(fp, "/Prev %d 0 R\r\n", other->index);
        for (other = object->next;
                other && other->bookmark.parent != object->bookmark.parent;
                other = other->next)
            ;
        if (other)
            fprintf(fp, "/Next %d 0 R\r\n", other->index);
        fprintf(fp, ">>\r\n");
        break;
    }

    case OBJ_outline: {
        struct pdf_object *first, *last, *cur;
        first = pdf_find_first_object(pdf, OBJ_bookmark);
        last = pdf_find_last_object(pdf, OBJ_bookmark);

        if (first && last) {
            int count = 0;
            cur = first;
            while (cur) {
                if (!cur->bookmark.parent)
                    count++;
                cur = cur->next;
            }

            /* Bookmark outline */
            fprintf(fp, "<<\r\n"
                    "/Count %d\r\n"
                    "/Type /Outlines\r\n"
                    "/First %d 0 R\r\n"
                    "/Last %d 0 R\r\n"
                    ">>\r\n",
                    count, first->index, last->index);
        }
        break;
    }

    case OBJ_font:
        fprintf(fp, "<<\r\n"
                "  /Type /Font\r\n"
                "  /Subtype /Type1\r\n"
                "  /BaseFont /%s\r\n"
                "  /Encoding /WinAnsiEncoding\r\n"
                ">>\r\n", object->font.name);
        break;

    case OBJ_pages: {
        struct pdf_object *page;
        int npages = 0;

        fprintf(fp, "<<\r\n"
                "/Type /Pages\r\n"
                "/Kids [ ");
        for (page = pdf_find_first_object(pdf, OBJ_page);
                page;
                page = page->next) {
            npages++;
            fprintf(fp, "%d 0 R ", page->index);
        }
        fprintf(fp, "]\r\n");
        fprintf(fp, "/Count %d\r\n", npages);
        fprintf(fp, ">>\r\n");
        break;
    }

    case OBJ_catalog: {
        struct pdf_object *outline = pdf_find_first_object(pdf, OBJ_outline);
        struct pdf_object *pages = pdf_find_first_object(pdf, OBJ_pages);

        fprintf(fp, "<<\r\n"
                "/Type /Catalog\r\n");
        if (outline)
            fprintf(fp,
                    "/Outlines %d 0 R\r\n"
                    "/PageMode /UseOutlines\r\n", outline->index);
        fprintf(fp, "/Pages %d 0 R\r\n"
                ">>\r\n",
                pages->index);
        break;
    }

    default:
        return pdf_set_err(pdf, -EINVAL, "Invalid PDF object type %d",
                           object->type);
    }

    fprintf(fp, "endobj\r\n");

    return 0;
}
