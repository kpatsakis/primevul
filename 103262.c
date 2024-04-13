gst_vorbis_tag_add (GstTagList * list, const gchar * tag, const gchar * value)
{
  const gchar *gst_tag;
  GType tag_type;

  g_return_if_fail (list != NULL);
  g_return_if_fail (tag != NULL);
  g_return_if_fail (value != NULL);

  g_return_if_fail (g_utf8_validate (tag, -1, NULL));
  g_return_if_fail (g_utf8_validate (value, -1, NULL));
  g_return_if_fail (strchr (tag, '=') == NULL);

  gst_tag = gst_tag_from_vorbis_tag (tag);
  if (gst_tag == NULL) {
    gchar *ext_comment;

    ext_comment = g_strdup_printf ("%s=%s", tag, value);
    gst_tag_list_add (list, GST_TAG_MERGE_APPEND, GST_TAG_EXTENDED_COMMENT,
        ext_comment, NULL);
    g_free (ext_comment);
    return;
  }

  tag_type = gst_tag_get_type (gst_tag);
  switch (tag_type) {
    case G_TYPE_UINT:{
      guint tmp;
      gchar *check;
      gboolean is_track_number_tag;
      gboolean is_disc_number_tag;

      is_track_number_tag = (strcmp (gst_tag, GST_TAG_TRACK_NUMBER) == 0);
      is_disc_number_tag = (strcmp (gst_tag, GST_TAG_ALBUM_VOLUME_NUMBER) == 0);
      tmp = strtoul (value, &check, 10);
      if (*check == '/' && (is_track_number_tag || is_disc_number_tag)) {
        guint count;

        check++;
        count = strtoul (check, &check, 10);
        if (*check != '\0' || count == 0)
          break;
        if (is_track_number_tag) {
          gst_tag_list_add (list, GST_TAG_MERGE_APPEND, GST_TAG_TRACK_COUNT,
              count, NULL);
        } else {
          gst_tag_list_add (list, GST_TAG_MERGE_APPEND,
              GST_TAG_ALBUM_VOLUME_COUNT, count, NULL);
        }
      }
      if (*check == '\0') {
        gst_tag_list_add (list, GST_TAG_MERGE_APPEND, gst_tag, tmp, NULL);
      }
      break;
    }
    case G_TYPE_STRING:{
      gchar *valid = NULL;

      /* specialcase for language code */
      if (strcmp (tag, "LANGUAGE") == 0) {
        const gchar *s = strchr (value, '[');

        /* Accept both ISO-639-1 and ISO-639-2 codes */
        if (s && strchr (s, ']') == s + 4) {
          valid = g_strndup (s + 1, 3);
        } else if (s && strchr (s, ']') == s + 3) {
          valid = g_strndup (s + 1, 2);
        } else if (strlen (value) != 2 && strlen (value) != 3) {
          GST_WARNING ("doesn't contain an ISO-639 language code: %s", value);
        }
      } else if (strcmp (tag, "LICENSE") == 0) {
        /* license tags in vorbis comments must contain an URI representing
         * the license and nothing more, at least according to:
         * http://wiki.xiph.org/index.php/LICENSE_and_COPYRIGHT_tags_on_Vorbis_Comments */
        if (value && gst_uri_is_valid (value))
          gst_tag = GST_TAG_LICENSE_URI;
      }

      if (!valid) {
        valid = g_strdup (value);
      }
      gst_tag_list_add (list, GST_TAG_MERGE_APPEND, gst_tag, valid, NULL);
      g_free (valid);
      break;
    }
    case G_TYPE_DOUBLE:{
      gchar *c;

      c = g_strdup (value);
      g_strdelimit (c, ",", '.');
      gst_tag_list_add (list, GST_TAG_MERGE_APPEND, gst_tag,
          g_strtod (c, NULL), NULL);
      g_free (c);
      break;
    }
    default:{
      if (tag_type == GST_TYPE_DATE) {
        guint y, d = 1, m = 1;
        gchar *check = (gchar *) value;

        y = strtoul (check, &check, 10);
        if (*check == '-') {
          check++;
          m = strtoul (check, &check, 10);
          if (*check == '-') {
            check++;
            d = strtoul (check, &check, 10);
          }
        }

        /* accept dates like 2007-00-00 and 2007-05-00 */
        if (y != 0) {
          if (m == 0 && d == 0)
            m = d = 1;
          else if (m != 0 && d == 0)
            d = 1;
        }

        /* date might be followed by a time */
        if ((*check == '\0' || g_ascii_isspace (*check)) && y != 0 &&
            g_date_valid_dmy (d, m, y)) {
          GDate *date;

          date = g_date_new_dmy (d, m, y);
          gst_tag_list_add (list, GST_TAG_MERGE_APPEND, gst_tag, date, NULL);
          g_date_free (date);
        } else {
          GST_DEBUG ("skipping invalid date '%s' (%u,%u,%u)", value, y, m, d);
        }
      } else {
        GST_WARNING ("Unhandled tag of type '%s' (%d)",
            g_type_name (tag_type), (gint) tag_type);
      }
      break;
    }
  }
 }
