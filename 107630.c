ssize_t libevt_record_values_read(
         libevt_record_values_t *record_values,
         libbfio_handle_t *file_io_handle,
         libevt_io_handle_t *io_handle,
         off64_t *file_offset,
         uint8_t strict_mode,
         libcerror_error_t **error )
{
	uint8_t record_size_data[ 4 ];

	uint8_t *record_data      = NULL;
	static char *function     = "libevt_record_values_read";
	size_t read_size          = 0;
	size_t record_data_offset = 0;
	ssize_t read_count        = 0;
	ssize_t total_read_count  = 0;
	uint32_t record_data_size = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( file_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file offset.",
		 function );

		return( -1 );
	}
	record_values->offset = *file_offset;

	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              record_size_data,
	              sizeof( uint32_t ),
	              error );

	if( read_count != (ssize_t) sizeof( uint32_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record size data.",
		 function );

		goto on_error;
	}
	*file_offset    += read_count;
	total_read_count = read_count;

	byte_stream_copy_to_uint32_little_endian(
	 record_size_data,
	 record_data_size );

	if( record_data_size < 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: record data size value out of bounds.",
		 function );

		goto on_error;
	}
#if SIZEOF_SIZE_T <= 4
	if( (size_t) record_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid record data size value exceeds maximum.",
		 function );

		goto on_error;
	}
#endif
	/* Allocating record data as 4 bytes and then using realloc here
	 * corrupts the memory
	 */
	record_data = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * record_data_size );

	if( record_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create record data.",
		 function );

		goto on_error;
	}
	byte_stream_copy_from_uint32_little_endian(
	 record_data,
	 record_data_size );

	record_data_offset = 4;

	read_size = record_data_size - record_data_offset;

	if( ( (size64_t) *file_offset + read_size ) > io_handle->file_size )
	{
		read_size = (size_t) ( io_handle->file_size - *file_offset );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              &( record_data[ record_data_offset ] ),
	              read_size,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record data.",
		 function );

		goto on_error;
	}
	*file_offset       += read_count;
	record_data_offset += read_count;
	total_read_count   += read_count;

	if( record_data_offset < (size_t) record_data_size )
	{
		if( libbfio_handle_seek_offset(
		     file_io_handle,
		     (off64_t) sizeof( evt_file_header_t ),
		     SEEK_SET,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek file header offset: %" PRIzd ".",
			 function,
			 sizeof( evt_file_header_t ) );

			goto on_error;
		}
		*file_offset = (off64_t) sizeof( evt_file_header_t );

		read_size = (size_t) record_data_size - record_data_offset;

		read_count = libbfio_handle_read_buffer(
		              file_io_handle,
		              &( record_data[ record_data_offset ] ),
		              read_size,
		              error );

		if( read_count != (ssize_t) read_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read record data.",
			 function );

			goto on_error;
		}
		*file_offset     += read_count;
		total_read_count += read_count;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: record data:\n",
		 function );
		libcnotify_print_data(
		 record_data,
		 (size_t) record_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     &( record_data[ 4 ] ),
	     evt_file_signature,
	     4 ) == 0 )
	{
		record_values->type = LIBEVT_RECORD_TYPE_EVENT;
	}
	else if( memory_compare(
	          &( record_data[ 4 ] ),
	          evt_end_of_file_record_signature1,
	          4 ) == 0 )
	{
		record_values->type = LIBEVT_RECORD_TYPE_END_OF_FILE;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record values signature.",
		 function );

		goto on_error;
	}
	if( record_values->type == LIBEVT_RECORD_TYPE_EVENT )
	{
		if( libevt_record_values_read_event(
		     record_values,
		     record_data,
		     (size_t) record_data_size,
		     strict_mode,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read event record values.",
			 function );

			goto on_error;
		}
	}
	else if( record_values->type == LIBEVT_RECORD_TYPE_END_OF_FILE )
	{
		if( libevt_record_values_read_end_of_file(
		     record_values,
		     record_data,
		     (size_t) record_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read end of file record values.",
			 function );

			goto on_error;
		}
	}
	memory_free(
	 record_data );

	return( total_read_count );

on_error:
	if( record_data != NULL )
	{
		memory_free(
		 record_data );
	}
	return( -1 );
}
