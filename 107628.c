int libevt_record_values_free(
     libevt_record_values_t **record_values,
     libcerror_error_t **error )
{
	static char *function = "libevt_record_values_free";
	int result            = 1;

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
	if( *record_values != NULL )
	{
		if( ( *record_values )->source_name != NULL )
		{
			if( libfvalue_value_free(
			     &( ( *record_values )->source_name ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free source name value.",
				 function );

				result = -1;
			}
		}
		if( ( *record_values )->computer_name != NULL )
		{
			if( libfvalue_value_free(
			     &( ( *record_values )->computer_name ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free computer name value.",
				 function );

				result = -1;
			}
		}
		if( ( *record_values )->user_security_identifier != NULL )
		{
			if( libfvalue_value_free(
			     &( ( *record_values )->user_security_identifier ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free user security identifier (SID).",
				 function );

				result = -1;
			}
		}
		if( ( *record_values )->strings != NULL )
		{
			if( libfvalue_value_free(
			     &( ( *record_values )->strings ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free strings.",
				 function );

				result = -1;
			}
		}
		if( ( *record_values )->data != NULL )
		{
			if( libfvalue_value_free(
			     &( ( *record_values )->data ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *record_values );

		*record_values = NULL;
	}
	return( result );
}
