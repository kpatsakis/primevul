int libevt_record_values_read_end_of_file(
     libevt_record_values_t *record_values,
     uint8_t *record_data,
     size_t record_data_size,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_read_end_of_file";
	uint32_t size         = 0;
	uint32_t size_copy    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
#endif

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
	if( record_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record data.",
		 function );

		return( -1 );
	}
	if( record_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid record data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( record_data_size < sizeof( evt_record_end_of_file_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: record data size value out of bounds.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_record_end_of_file_t *) record_data )->size,
	 size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evt_record_end_of_file_t *) record_data )->size_copy,
	 size_copy );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->signature1,
		 value_32bit );
		libcnotify_printf(
		 "%s: signature1\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->signature2,
		 value_32bit );
		libcnotify_printf(
		 "%s: signature2\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->signature3,
		 value_32bit );
		libcnotify_printf(
		 "%s: signature3\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->signature4,
		 value_32bit );
		libcnotify_printf(
		 "%s: signature4\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->first_record_offset,
		 value_32bit );
		libcnotify_printf(
		 "%s: first record offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->end_of_file_record_offset,
		 value_32bit );
		libcnotify_printf(
		 "%s: end of file record offset\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->last_record_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: last record number\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evt_record_end_of_file_t *) record_data )->first_record_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: first record number\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: size copy\t\t\t: %" PRIu32 "\n",
		 function,
		 size_copy );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( size != size_copy )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for size and size copy.",
		 function );

		return( -1 );
	}
	if( record_data_size != (size_t) size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for record data size and size.",
		 function );

		return( -1 );
	}
/* TODO correct values in IO handle if necessary */

	return( 1 );
}
