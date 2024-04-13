int afReadFrames (AFfilehandle file, int trackid, void *samples,
	int nvframeswanted)
{
	_Track	*track;
	_AFmoduleinst	*firstmod;
	_AFchunk	*userc;
	AFframecount	nvframesleft, nvframes2read;
	int		bytes_per_vframe;
	AFframecount	vframe;

	if (!_af_filehandle_ok(file))
		return -1;

	if (!_af_filehandle_can_read(file))
		return -1;

	if ((track = _af_filehandle_get_track(file, trackid)) == NULL)
		return -1;

	if (track->ms.modulesdirty)
	{
		if (_AFsetupmodules(file, track) != AF_SUCCEED)
			return -1;
	}

	/*if (file->seekok) {*/

	if (af_fseek(file->fh, track->fpos_next_frame, SEEK_SET) < 0)
	{
		_af_error(AF_BAD_LSEEK, "unable to position read pointer at next frame");
		return -1;
	}

	/* } */

	if (track->totalvframes == -1)
		nvframes2read = nvframeswanted;
	else
	{
		nvframesleft = track->totalvframes - track->nextvframe;
		nvframes2read = (nvframeswanted > nvframesleft) ?
			nvframesleft : nvframeswanted;
	}
	bytes_per_vframe = _af_format_frame_size(&track->v, true);

	firstmod = &track->ms.module[track->ms.nmodules-1];
	userc = &track->ms.chunk[track->ms.nmodules];

	track->filemodhappy = true;

	vframe = 0;

	if (!track->ms.mustuseatomicnvframes)
	{
		assert(track->frames2ignore == 0);
		userc->buf = samples;
		userc->nframes = nvframes2read;

		(*firstmod->mod->run_pull)(firstmod);
		if (track->filemodhappy)
			vframe += userc->nframes;
	}
	else
	{
		bool	eof = false;

		if (track->frames2ignore != 0)
		{
			userc->nframes = track->frames2ignore;
			userc->buf = _af_malloc(track->frames2ignore * bytes_per_vframe);
			if (userc->buf == AF_NULL)
				return 0;

			(*firstmod->mod->run_pull)(firstmod);

			/* Have we hit EOF? */
			if (userc->nframes < track->frames2ignore)
				eof = true;

			track->frames2ignore = 0;

			free(userc->buf);
			userc->buf = NULL;
		}

		/*
			Now start reading useful frames, until EOF or
			premature EOF.
		*/

		while (track->filemodhappy && !eof && vframe < nvframes2read)
		{
			AFframecount	nvframes2pull;
			userc->buf = (char *) samples + bytes_per_vframe * vframe;

			if (vframe <= nvframes2read - _AF_ATOMIC_NVFRAMES)
				nvframes2pull = _AF_ATOMIC_NVFRAMES;
			else
				nvframes2pull = nvframes2read - vframe;

			userc->nframes = nvframes2pull;

			(*firstmod->mod->run_pull)(firstmod);

			if (track->filemodhappy)
			{
				vframe += userc->nframes;
				if (userc->nframes < nvframes2pull)
					eof = true;
			}
		}
	}

	track->nextvframe += vframe;

	return vframe;
}