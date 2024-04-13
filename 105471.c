drive_benchmark_completed_cb (DBusGMethodInvocation *context,
                              Device *device,
                              gboolean job_was_cancelled,
                              int status,
                              const gchar *stderr,
                              const gchar *stdout,
                              gpointer user_data)
{
  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {
      GPtrArray *read_transfer_rate_array;
      GPtrArray *write_transfer_rate_array;
      GPtrArray *access_time_array;
      gchar **lines;
      guint n;
      GType elem_gtype;

      elem_gtype = dbus_g_type_get_struct ("GValueArray", G_TYPE_UINT64, G_TYPE_DOUBLE, G_TYPE_INVALID);

      read_transfer_rate_array = g_ptr_array_new_with_free_func ((GDestroyNotify) g_value_array_free);
      write_transfer_rate_array = g_ptr_array_new_with_free_func ((GDestroyNotify) g_value_array_free);
      access_time_array = g_ptr_array_new_with_free_func ((GDestroyNotify) g_value_array_free);

      lines = g_strsplit (stdout, "\n", 0);
      for (n = 0; lines != NULL && lines[n] != NULL; n++)
        {
          const gchar *line = lines[n];
          guint64 offset;
          gdouble rate;
          gdouble access_time;
          GValue elem =
            { 0 };

          if (sscanf (line, "read_transfer_rate: offset %" G_GUINT64_FORMAT " rate %lf",
                      &offset,
                      &rate) == 2)
            {

              g_value_init (&elem, elem_gtype);
              g_value_take_boxed (&elem, dbus_g_type_specialized_construct (elem_gtype));
              dbus_g_type_struct_set (&elem, 0, offset, 1, rate, G_MAXUINT);
              g_ptr_array_add (read_transfer_rate_array, g_value_get_boxed (&elem));

            }
          else if (sscanf (line, "write_transfer_rate: offset %" G_GUINT64_FORMAT " rate %lf",
                           &offset,
                           &rate) == 2)
            {

              g_value_init (&elem, elem_gtype);
              g_value_take_boxed (&elem, dbus_g_type_specialized_construct (elem_gtype));
              dbus_g_type_struct_set (&elem, 0, offset, 1, rate, G_MAXUINT);
              g_ptr_array_add (write_transfer_rate_array, g_value_get_boxed (&elem));

            }
          else if (sscanf (line, "access_time: offset %" G_GUINT64_FORMAT " time %lf",
                           &offset,
                           &access_time) == 2)
            {

              g_value_init (&elem, elem_gtype);
              g_value_take_boxed (&elem, dbus_g_type_specialized_construct (elem_gtype));
              dbus_g_type_struct_set (&elem, 0, offset, 1, access_time, G_MAXUINT);
              g_ptr_array_add (access_time_array, g_value_get_boxed (&elem));

            }
          else
            {
              g_warning ("unhandled line %d: `%s'", n, line);
            }

        }
      g_strfreev (lines);

      dbus_g_method_return (context, read_transfer_rate_array, write_transfer_rate_array, access_time_array);

      g_ptr_array_unref (read_transfer_rate_array);
      g_ptr_array_unref (write_transfer_rate_array);
      g_ptr_array_unref (access_time_array);
    }
  else
    {
      if (job_was_cancelled)
        {
          throw_error (context, ERROR_CANCELLED, "Job was cancelled");
        }
      else
        {
          throw_error (context,
                       ERROR_FAILED,
                       "Error benchmarking: helper exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
    }
}
