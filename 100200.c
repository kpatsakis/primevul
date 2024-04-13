sf_command	(SNDFILE *sndfile, int command, void *data, int datasize)
{	SF_PRIVATE *psf = (SF_PRIVATE *) sndfile ;
	double quality ;
	int old_value ;

	/* This set of commands do not need the sndfile parameter. */
	switch (command)
	{	case SFC_GET_LIB_VERSION :
			if (data == NULL)
			{	if (psf)
					psf->error = SFE_BAD_COMMAND_PARAM ;
				return SFE_BAD_COMMAND_PARAM ;
				} ;
			snprintf (data, datasize, "%s", sf_version_string ()) ;
			return strlen (data) ;

		case SFC_GET_SIMPLE_FORMAT_COUNT :
			if (data == NULL || datasize != SIGNED_SIZEOF (int))
				return (sf_errno = SFE_BAD_COMMAND_PARAM) ;
			*((int*) data) = psf_get_format_simple_count () ;
			return 0 ;

		case SFC_GET_SIMPLE_FORMAT :
			if (data == NULL || datasize != SIGNED_SIZEOF (SF_FORMAT_INFO))
				return (sf_errno = SFE_BAD_COMMAND_PARAM) ;
			return psf_get_format_simple (data) ;

		case SFC_GET_FORMAT_MAJOR_COUNT :
			if (data == NULL || datasize != SIGNED_SIZEOF (int))
				return (sf_errno = SFE_BAD_COMMAND_PARAM) ;
			*((int*) data) = psf_get_format_major_count () ;
			return 0 ;

		case SFC_GET_FORMAT_MAJOR :
			if (data == NULL || datasize != SIGNED_SIZEOF (SF_FORMAT_INFO))
				return (sf_errno = SFE_BAD_COMMAND_PARAM) ;
			return psf_get_format_major (data) ;

		case SFC_GET_FORMAT_SUBTYPE_COUNT :
			if (data == NULL || datasize != SIGNED_SIZEOF (int))
				return (sf_errno = SFE_BAD_COMMAND_PARAM) ;
			*((int*) data) = psf_get_format_subtype_count () ;
			return 0 ;

		case SFC_GET_FORMAT_SUBTYPE :
			if (data == NULL || datasize != SIGNED_SIZEOF (SF_FORMAT_INFO))
				return (sf_errno = SFE_BAD_COMMAND_PARAM) ;
			return psf_get_format_subtype (data) ;

		case SFC_GET_FORMAT_INFO :
			if (data == NULL || datasize != SIGNED_SIZEOF (SF_FORMAT_INFO))
				return (sf_errno = SFE_BAD_COMMAND_PARAM) ;
			return psf_get_format_info (data) ;
		} ;

	if (sndfile == NULL && command == SFC_GET_LOG_INFO)
	{	if (data == NULL)
			return (sf_errno = SFE_BAD_COMMAND_PARAM) ;
		snprintf (data, datasize, "%s", sf_parselog) ;
		return strlen (data) ;
		} ;

	VALIDATE_SNDFILE_AND_ASSIGN_PSF (sndfile, psf, 1) ;

	switch (command)
	{	case SFC_SET_NORM_FLOAT :
			old_value = psf->norm_float ;
			psf->norm_float = (datasize) ? SF_TRUE : SF_FALSE ;
			return old_value ;

		case SFC_GET_CURRENT_SF_INFO :
			if (data == NULL || datasize != SIGNED_SIZEOF (SF_INFO))
				return (sf_errno = SFE_BAD_COMMAND_PARAM) ;
			memcpy (data, &psf->sf, sizeof (SF_INFO)) ;
			break ;

		case SFC_SET_NORM_DOUBLE :
			old_value = psf->norm_double ;
			psf->norm_double = (datasize) ? SF_TRUE : SF_FALSE ;
			return old_value ;

		case SFC_GET_NORM_FLOAT :
			return psf->norm_float ;

		case SFC_GET_NORM_DOUBLE :
			return psf->norm_double ;

		case SFC_SET_SCALE_FLOAT_INT_READ :
			old_value = psf->float_int_mult ;

			psf->float_int_mult = (datasize != 0) ? SF_TRUE : SF_FALSE ;
			if (psf->float_int_mult && psf->float_max < 0.0)
				/* Scale to prevent wrap-around distortion. */
				psf->float_max = (32768.0 / 32767.0) * psf_calc_signal_max (psf, SF_FALSE) ;
			return old_value ;

		case SFC_SET_SCALE_INT_FLOAT_WRITE :
			old_value = psf->scale_int_float ;
			psf->scale_int_float = (datasize != 0) ? SF_TRUE : SF_FALSE ;
			return old_value ;

		case SFC_SET_ADD_PEAK_CHUNK :
			{	int format = SF_CONTAINER (psf->sf.format) ;

				/* Only WAV and AIFF support the PEAK chunk. */
				switch (format)
				{	case SF_FORMAT_AIFF :
					case SF_FORMAT_CAF :
					case SF_FORMAT_WAV :
					case SF_FORMAT_WAVEX :
					case SF_FORMAT_RF64 :
						break ;

					default :
						return SF_FALSE ;
					} ;

				format = SF_CODEC (psf->sf.format) ;

				/* Only files containg the following data types support the PEAK chunk. */
				if (format != SF_FORMAT_FLOAT && format != SF_FORMAT_DOUBLE)
					return SF_FALSE ;

				} ;
			/* Can only do this is in SFM_WRITE mode. */
			if (psf->file.mode != SFM_WRITE && psf->file.mode != SFM_RDWR)
				return SF_FALSE ;
			/* If data has already been written this must fail. */
			if (psf->have_written)
			{	psf->error = SFE_CMD_HAS_DATA ;
				return SF_FALSE ;
				} ;
			/* Everything seems OK, so set psf->has_peak and re-write header. */
			if (datasize == SF_FALSE && psf->peak_info != NULL)
			{	free (psf->peak_info) ;
				psf->peak_info = NULL ;
				}
			else if (psf->peak_info == NULL)
			{	psf->peak_info = peak_info_calloc (psf->sf.channels) ;
				if (psf->peak_info != NULL)
					psf->peak_info->peak_loc = SF_PEAK_START ;
				} ;

			if (psf->write_header)
				psf->write_header (psf, SF_TRUE) ;
			return datasize ;

		case SFC_SET_ADD_HEADER_PAD_CHUNK :
			return SF_FALSE ;

		case SFC_GET_LOG_INFO :
			if (data == NULL)
				return SFE_BAD_COMMAND_PARAM ;
			snprintf (data, datasize, "%s", psf->parselog.buf) ;
			break ;

		case SFC_CALC_SIGNAL_MAX :
			if (data == NULL || datasize != sizeof (double))
				return (psf->error = SFE_BAD_COMMAND_PARAM) ;
			*((double*) data) = psf_calc_signal_max (psf, SF_FALSE) ;
			break ;

		case SFC_CALC_NORM_SIGNAL_MAX :
			if (data == NULL || datasize != sizeof (double))
				return (psf->error = SFE_BAD_COMMAND_PARAM) ;
			*((double*) data) = psf_calc_signal_max (psf, SF_TRUE) ;
			break ;

		case SFC_CALC_MAX_ALL_CHANNELS :
			if (data == NULL || datasize != SIGNED_SIZEOF (double) * psf->sf.channels)
				return (psf->error = SFE_BAD_COMMAND_PARAM) ;
			return psf_calc_max_all_channels (psf, (double*) data, SF_FALSE) ;

		case SFC_CALC_NORM_MAX_ALL_CHANNELS :
			if (data == NULL || datasize != SIGNED_SIZEOF (double) * psf->sf.channels)
				return (psf->error = SFE_BAD_COMMAND_PARAM) ;
			return psf_calc_max_all_channels (psf, (double*) data, SF_TRUE) ;

		case SFC_GET_SIGNAL_MAX :
			if (data == NULL || datasize != sizeof (double))
			{	psf->error = SFE_BAD_COMMAND_PARAM ;
				return SF_FALSE ;
				} ;
			return psf_get_signal_max (psf, (double *) data) ;

		case SFC_GET_MAX_ALL_CHANNELS :
			if (data == NULL || datasize != SIGNED_SIZEOF (double) * psf->sf.channels)
			{	psf->error = SFE_BAD_COMMAND_PARAM ;
				return SF_FALSE ;
				} ;
			return psf_get_max_all_channels (psf, (double*) data) ;

		case SFC_UPDATE_HEADER_NOW :
			if (psf->write_header)
				psf->write_header (psf, SF_TRUE) ;
			break ;

		case SFC_SET_UPDATE_HEADER_AUTO :
			psf->auto_header = datasize ? SF_TRUE : SF_FALSE ;
			return psf->auto_header ;
			break ;

		case SFC_SET_ADD_DITHER_ON_WRITE :
		case SFC_SET_ADD_DITHER_ON_READ :
			/*
			** FIXME !
			** These are obsolete. Just return.
			** Remove some time after version 1.0.8.
			*/
			break ;

		case SFC_SET_DITHER_ON_WRITE :
			if (data == NULL || datasize != SIGNED_SIZEOF (SF_DITHER_INFO))
				return (psf->error = SFE_BAD_COMMAND_PARAM) ;
			memcpy (&psf->write_dither, data, sizeof (psf->write_dither)) ;
			if (psf->file.mode == SFM_WRITE || psf->file.mode == SFM_RDWR)
				dither_init (psf, SFM_WRITE) ;
			break ;

		case SFC_SET_DITHER_ON_READ :
			if (data == NULL || datasize != SIGNED_SIZEOF (SF_DITHER_INFO))
				return (psf->error = SFE_BAD_COMMAND_PARAM) ;
			memcpy (&psf->read_dither, data, sizeof (psf->read_dither)) ;
			if (psf->file.mode == SFM_READ || psf->file.mode == SFM_RDWR)
				dither_init (psf, SFM_READ) ;
			break ;

		case SFC_FILE_TRUNCATE :
			if (psf->file.mode != SFM_WRITE && psf->file.mode != SFM_RDWR)
				return SF_TRUE ;
			if (datasize != sizeof (sf_count_t))
				return SF_TRUE ;
			if (data == NULL || datasize != sizeof (sf_count_t))
			{	psf->error = SFE_BAD_COMMAND_PARAM ;
				return SF_FALSE ;
				}
			else
			{	sf_count_t position ;

				position = *((sf_count_t*) data) ;

				if (sf_seek (sndfile, position, SEEK_SET) != position)
					return SF_TRUE ;

				psf->sf.frames = position ;

				position = psf_fseek (psf, 0, SEEK_CUR) ;

				return psf_ftruncate (psf, position) ;
				} ;
			break ;

		case SFC_SET_RAW_START_OFFSET :
			if (data == NULL || datasize != sizeof (sf_count_t))
				return (psf->error = SFE_BAD_COMMAND_PARAM) ;

			if ((SF_CONTAINER (psf->sf.format)) != SF_FORMAT_RAW)
				return (psf->error = SFE_BAD_COMMAND_PARAM) ;

			psf->dataoffset = *((sf_count_t*) data) ;
			sf_seek (sndfile, 0, SEEK_CUR) ;
			break ;

		case SFC_GET_EMBED_FILE_INFO :
			if (data == NULL || datasize != sizeof (SF_EMBED_FILE_INFO))
				return (psf->error = SFE_BAD_COMMAND_PARAM) ;

			((SF_EMBED_FILE_INFO*) data)->offset = psf->fileoffset ;
			((SF_EMBED_FILE_INFO*) data)->length = psf->filelength ;
			break ;

		/* Lite remove start */
		case SFC_TEST_IEEE_FLOAT_REPLACE :
			psf->ieee_replace = (datasize) ? SF_TRUE : SF_FALSE ;
			if ((SF_CODEC (psf->sf.format)) == SF_FORMAT_FLOAT)
				float32_init (psf) ;
			else if ((SF_CODEC (psf->sf.format)) == SF_FORMAT_DOUBLE)
				double64_init (psf) ;
			else
				return (psf->error = SFE_BAD_COMMAND_PARAM) ;
			break ;
		/* Lite remove end */

		case SFC_SET_CLIPPING :
			psf->add_clipping = (datasize) ? SF_TRUE : SF_FALSE ;
			return psf->add_clipping ;

		case SFC_GET_CLIPPING :
			return psf->add_clipping ;

		case SFC_GET_LOOP_INFO :
			if (datasize != sizeof (SF_LOOP_INFO) || data == NULL)
			{	psf->error = SFE_BAD_COMMAND_PARAM ;
				return SF_FALSE ;
				} ;
			if (psf->loop_info == NULL)
				return SF_FALSE ;
			memcpy (data, psf->loop_info, sizeof (SF_LOOP_INFO)) ;
			return SF_TRUE ;

		case SFC_SET_BROADCAST_INFO :
			{	int format = SF_CONTAINER (psf->sf.format) ;

				/* Only WAV and RF64 supports the BEXT (Broadcast) chunk. */
				if (format != SF_FORMAT_WAV && format != SF_FORMAT_WAVEX && format != SF_FORMAT_RF64)
					return SF_FALSE ;
				} ;

			/* Only makes sense in SFM_WRITE or SFM_RDWR mode. */
			if ((psf->file.mode != SFM_WRITE) && (psf->file.mode != SFM_RDWR))
				return SF_FALSE ;
			/* If data has already been written this must fail. */
			if (psf->broadcast_16k == NULL && psf->have_written)
			{	psf->error = SFE_CMD_HAS_DATA ;
				return SF_FALSE ;
				} ;

			if (NOT (broadcast_var_set (psf, data, datasize)))
				return SF_FALSE ;

			if (psf->write_header)
				psf->write_header (psf, SF_TRUE) ;
			return SF_TRUE ;

		case SFC_GET_BROADCAST_INFO :
			if (data == NULL)
			{	psf->error = SFE_BAD_COMMAND_PARAM ;
				return SF_FALSE ;
				} ;
			return broadcast_var_get (psf, data, datasize) ;

		case SFC_SET_CART_INFO :
			{	int format = SF_CONTAINER (psf->sf.format) ;
				/* Only WAV and RF64 support cart chunk format */
				if (format != SF_FORMAT_WAV && format != SF_FORMAT_RF64)
					return SF_FALSE ;
				} ;

			/* Only makes sense in SFM_WRITE or SFM_RDWR mode */
			if ((psf->file.mode != SFM_WRITE) && (psf->file.mode != SFM_RDWR))
				return SF_FALSE ;
			/* If data has already been written this must fail. */
			if (psf->cart_16k == NULL && psf->have_written)
			{	psf->error = SFE_CMD_HAS_DATA ;
				return SF_FALSE ;
				} ;
			if (NOT (cart_var_set (psf, data, datasize)))
				return SF_FALSE ;
			if (psf->write_header)
				psf->write_header (psf, SF_TRUE) ;
			return SF_TRUE ;

		case SFC_GET_CART_INFO :
			if (data == NULL)
			{	psf->error = SFE_BAD_COMMAND_PARAM ;
				return SF_FALSE ;
				} ;
			return cart_var_get (psf, data, datasize) ;

		case SFC_GET_CUE_COUNT :
			if (datasize != sizeof (uint32_t) || data == NULL)
			{	psf->error = SFE_BAD_COMMAND_PARAM ;
				return SF_FALSE ;
				} ;
			if (psf->cues != NULL)
			{	*((uint32_t *) data) = psf->cues->cue_count ;
				return SF_TRUE ;
				} ;
			return SF_FALSE ;

		case SFC_GET_CUE :
			if (datasize != sizeof (SF_CUES) || data == NULL)
			{	psf->error = SFE_BAD_COMMAND_PARAM ;
				return SF_FALSE ;
				} ;
			if (psf->cues == NULL)
				return SF_FALSE ;
			psf_get_cues (psf, data, datasize) ;
			return SF_TRUE ;

		case SFC_SET_CUE :
			if (psf->have_written)
			{	psf->error = SFE_CMD_HAS_DATA ;
				return SF_FALSE ;
				} ;
			if (datasize != sizeof (SF_CUES) || data == NULL)
			{	psf->error = SFE_BAD_COMMAND_PARAM ;
				return SF_FALSE ;
				} ;

			if (psf->cues == NULL && (psf->cues = psf_cues_dup (data)) == NULL)
			{	psf->error = SFE_MALLOC_FAILED ;
				return SF_FALSE ;
				} ;
			return SF_TRUE ;

		case SFC_GET_INSTRUMENT :
			if (datasize != sizeof (SF_INSTRUMENT) || data == NULL)
			{	psf->error = SFE_BAD_COMMAND_PARAM ;
				return SF_FALSE ;
				} ;
			if (psf->instrument == NULL)
				return SF_FALSE ;
			memcpy (data, psf->instrument, sizeof (SF_INSTRUMENT)) ;
			return SF_TRUE ;

		case SFC_SET_INSTRUMENT :
			/* If data has already been written this must fail. */
			if (psf->have_written)
			{	psf->error = SFE_CMD_HAS_DATA ;
				return SF_FALSE ;
				} ;
			if (datasize != sizeof (SF_INSTRUMENT) || data == NULL)
			{	psf->error = SFE_BAD_COMMAND_PARAM ;
				return SF_FALSE ;
				} ;

			if (psf->instrument == NULL && (psf->instrument = psf_instrument_alloc ()) == NULL)
			{	psf->error = SFE_MALLOC_FAILED ;
				return SF_FALSE ;
				} ;
			memcpy (psf->instrument, data, sizeof (SF_INSTRUMENT)) ;
			return SF_TRUE ;

		case SFC_RAW_DATA_NEEDS_ENDSWAP :
			return psf->data_endswap ;

		case SFC_GET_CHANNEL_MAP_INFO :
			if (psf->channel_map == NULL)
				return SF_FALSE ;

			if (data == NULL || datasize != SIGNED_SIZEOF (psf->channel_map [0]) * psf->sf.channels)
			{	psf->error = SFE_BAD_COMMAND_PARAM ;
				return SF_FALSE ;
				} ;

			memcpy (data, psf->channel_map, datasize) ;
			return SF_TRUE ;

		case SFC_SET_CHANNEL_MAP_INFO :
			if (psf->have_written)
			{	psf->error = SFE_CMD_HAS_DATA ;
				return SF_FALSE ;
				} ;
			if (data == NULL || datasize != SIGNED_SIZEOF (psf->channel_map [0]) * psf->sf.channels)
			{	psf->error = SFE_BAD_COMMAND_PARAM ;
				return SF_FALSE ;
				} ;

			{	int *iptr ;

				for (iptr = data ; iptr < (int*) data + psf->sf.channels ; iptr++)
				{	if (*iptr <= SF_CHANNEL_MAP_INVALID || *iptr >= SF_CHANNEL_MAP_MAX)
					{	psf->error = SFE_BAD_COMMAND_PARAM ;
						return SF_FALSE ;
						} ;
					} ;
				} ;

			free (psf->channel_map) ;
			if ((psf->channel_map = malloc (datasize)) == NULL)
			{	psf->error = SFE_MALLOC_FAILED ;
				return SF_FALSE ;
				} ;

			memcpy (psf->channel_map, data, datasize) ;

			/*
			**	Pass the command down to the container's command handler.
			**	Don't pass user data, use validated psf->channel_map data instead.
			*/
			if (psf->command)
				return psf->command (psf, command, NULL, 0) ;
			return SF_FALSE ;

		case SFC_SET_VBR_ENCODING_QUALITY :
			if (data == NULL || datasize != sizeof (double))
				return SF_FALSE ;

			quality = *((double *) data) ;
			quality = 1.0 - SF_MAX (0.0, SF_MIN (1.0, quality)) ;
			return sf_command (sndfile, SFC_SET_COMPRESSION_LEVEL, &quality, sizeof (quality)) ;


		default :
			/* Must be a file specific command. Pass it on. */
			if (psf->command)
				return psf->command (psf, command, data, datasize) ;

			psf_log_printf (psf, "*** sf_command : cmd = 0x%X\n", command) ;
			return (psf->error = SFE_BAD_COMMAND_PARAM) ;
		} ;

	return 0 ;
} /* sf_command */
