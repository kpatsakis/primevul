int libevt_record_values_read_element_data(
     libevt_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *element,
     libfcache_cache_t *cache,
     int element_file_index LIBEVT_ATTRIBUTE_UNUSED,
     off64_t element_offset,
     size64_t element_size LIBEVT_ATTRIBUTE_UNUSED,
     uint32_t element_flags LIBEVT_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBEVT_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libevt_record_values_t *record_values = NULL;
	static char *function                 = "libevt_record_values_read_element_data";
	off64_t file_offset                   = 0;
	ssize_t read_count                    = 0;

	LIBEVT_UNREFERENCED_PARAMETER( element_size )
	LIBEVT_UNREFERENCED_PARAMETER( element_file_index )
	LIBEVT_UNREFERENCED_PARAMETER( element_flags )
	LIBEVT_UNREFERENCED_PARAMETER( read_flags )

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading record  at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 element_offset,
		 element_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     element_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek record offset: %" PRIi64 ".",
		 function,
		 element_offset );

		goto on_error;
	}
	if( libevt_record_values_initialize(
	     &record_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create record values.",
		 function );

		goto on_error;
	}
	/* File offset must be before being passed to libevt_record_values_read
	 */
	file_offset = element_offset;

	read_count = libevt_record_values_read(
		      record_values,
		      file_io_handle,
		      io_handle,
		      &file_offset,
		      0,
		      error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record at offset: %" PRIi64 ".",
		 function,
		 element_offset );

		goto on_error;
	}
	if( libfdata_list_element_set_element_value(
	     element,
	     (intptr_t *) file_io_handle,
	     cache,
	     (intptr_t *) record_values,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libevt_record_values_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set record values as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( record_values != NULL )
	{
		libevt_record_values_free(
		 &record_values,
		 NULL );
	}
	return( -1 );
}
