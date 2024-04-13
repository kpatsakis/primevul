update_local_clock(peer_t *p)
{
	int rc;
	struct timex tmx;
	/* Note: can use G.cluster_offset instead: */
	double offset = p->filter_offset;
	double recv_time = p->lastpkt_recv_time;
	double abs_offset;
#if !USING_KERNEL_PLL_LOOP
	double freq_drift;
#endif
#if !USING_KERNEL_PLL_LOOP || USING_INITIAL_FREQ_ESTIMATION
	double since_last_update;
#endif
	double etemp, dtemp;

	abs_offset = fabs(offset);

#if 0
	/* If needed, -S script can do it by looking at $offset
	 * env var and killing parent */
	/* If the offset is too large, give up and go home */
	if (abs_offset > PANIC_THRESHOLD) {
		bb_error_msg_and_die("offset %f far too big, exiting", offset);
	}
#endif

	/* If this is an old update, for instance as the result
	 * of a system peer change, avoid it. We never use
	 * an old sample or the same sample twice.
	 */
	if (recv_time <= G.last_update_recv_time) {
		VERB3 bb_error_msg("update from %s: same or older datapoint, not using it",
			p->p_dotted);
		return 0; /* "leave poll interval as is" */
	}

	/* Clock state machine transition function. This is where the
	 * action is and defines how the system reacts to large time
	 * and frequency errors.
	 */
#if !USING_KERNEL_PLL_LOOP || USING_INITIAL_FREQ_ESTIMATION
	since_last_update = recv_time - G.reftime;
#endif
#if !USING_KERNEL_PLL_LOOP
	freq_drift = 0;
#endif
#if USING_INITIAL_FREQ_ESTIMATION
	if (G.discipline_state == STATE_FREQ) {
		/* Ignore updates until the stepout threshold */
		if (since_last_update < WATCH_THRESHOLD) {
			VERB4 bb_error_msg("measuring drift, datapoint ignored, %f sec remains",
					WATCH_THRESHOLD - since_last_update);
			return 0; /* "leave poll interval as is" */
		}
# if !USING_KERNEL_PLL_LOOP
		freq_drift = (offset - G.last_update_offset) / since_last_update;
# endif
	}
#endif

	/* There are two main regimes: when the
	 * offset exceeds the step threshold and when it does not.
	 */
	if (abs_offset > STEP_THRESHOLD) {
#if 0
		double remains;

		switch (G.discipline_state) {
		case STATE_SYNC:
			/* The first outlyer: ignore it, switch to SPIK state */
			VERB3 bb_error_msg("update from %s: offset:%+f, spike%s",
				p->p_dotted, offset,
				"");
			G.discipline_state = STATE_SPIK;
			return -1; /* "decrease poll interval" */

		case STATE_SPIK:
			/* Ignore succeeding outlyers until either an inlyer
			 * is found or the stepout threshold is exceeded.
			 */
			remains = WATCH_THRESHOLD - since_last_update;
			if (remains > 0) {
				VERB3 bb_error_msg("update from %s: offset:%+f, spike%s",
					p->p_dotted, offset,
					", datapoint ignored");
				return -1; /* "decrease poll interval" */
			}
			/* fall through: we need to step */
		} /* switch */
#endif

		/* Step the time and clamp down the poll interval.
		 *
		 * In NSET state an initial frequency correction is
		 * not available, usually because the frequency file has
		 * not yet been written. Since the time is outside the
		 * capture range, the clock is stepped. The frequency
		 * will be set directly following the stepout interval.
		 *
		 * In FSET state the initial frequency has been set
		 * from the frequency file. Since the time is outside
		 * the capture range, the clock is stepped immediately,
		 * rather than after the stepout interval. Guys get
		 * nervous if it takes 17 minutes to set the clock for
		 * the first time.
		 *
		 * In SPIK state the stepout threshold has expired and
		 * the phase is still above the step threshold. Note
		 * that a single spike greater than the step threshold
		 * is always suppressed, even at the longer poll
		 * intervals.
		 */
		VERB4 bb_error_msg("stepping time by %+f; poll_exp=MINPOLL", offset);
		step_time(offset);
		if (option_mask32 & OPT_q) {
			/* We were only asked to set time once. Done. */
			exit(0);
		}

		clamp_pollexp_and_set_MAXSTRAT();

		run_script("step", offset);

		recv_time += offset;

#if USING_INITIAL_FREQ_ESTIMATION
		if (G.discipline_state == STATE_NSET) {
			set_new_values(STATE_FREQ, /*offset:*/ 0, recv_time);
			return 1; /* "ok to increase poll interval" */
		}
#endif
		abs_offset = offset = 0;
		set_new_values(STATE_SYNC, offset, recv_time);
	} else { /* abs_offset <= STEP_THRESHOLD */

		/* The ratio is calculated before jitter is updated to make
		 * poll adjust code more sensitive to large offsets.
		 */
		G.offset_to_jitter_ratio = abs_offset / G.discipline_jitter;

		/* Compute the clock jitter as the RMS of exponentially
		 * weighted offset differences. Used by the poll adjust code.
		 */
		etemp = SQUARE(G.discipline_jitter);
		dtemp = SQUARE(offset - G.last_update_offset);
		G.discipline_jitter = SQRT(etemp + (dtemp - etemp) / AVG);
		if (G.discipline_jitter < G_precision_sec)
			G.discipline_jitter = G_precision_sec;

		switch (G.discipline_state) {
		case STATE_NSET:
			if (option_mask32 & OPT_q) {
				/* We were only asked to set time once.
				 * The clock is precise enough, no need to step.
				 */
				exit(0);
			}
#if USING_INITIAL_FREQ_ESTIMATION
			/* This is the first update received and the frequency
			 * has not been initialized. The first thing to do
			 * is directly measure the oscillator frequency.
			 */
			set_new_values(STATE_FREQ, offset, recv_time);
#else
			set_new_values(STATE_SYNC, offset, recv_time);
#endif
			VERB4 bb_error_msg("transitioning to FREQ, datapoint ignored");
			return 0; /* "leave poll interval as is" */

#if 0 /* this is dead code for now */
		case STATE_FSET:
			/* This is the first update and the frequency
			 * has been initialized. Adjust the phase, but
			 * don't adjust the frequency until the next update.
			 */
			set_new_values(STATE_SYNC, offset, recv_time);
			/* freq_drift remains 0 */
			break;
#endif

#if USING_INITIAL_FREQ_ESTIMATION
		case STATE_FREQ:
			/* since_last_update >= WATCH_THRESHOLD, we waited enough.
			 * Correct the phase and frequency and switch to SYNC state.
			 * freq_drift was already estimated (see code above)
			 */
			set_new_values(STATE_SYNC, offset, recv_time);
			break;
#endif

		default:
#if !USING_KERNEL_PLL_LOOP
			/* Compute freq_drift due to PLL and FLL contributions.
			 *
			 * The FLL and PLL frequency gain constants
			 * depend on the poll interval and Allan
			 * intercept. The FLL is not used below one-half
			 * the Allan intercept. Above that the loop gain
			 * increases in steps to 1 / AVG.
			 */
			if ((1 << G.poll_exp) > ALLAN / 2) {
				etemp = FLL - G.poll_exp;
				if (etemp < AVG)
					etemp = AVG;
				freq_drift += (offset - G.last_update_offset) / (MAXD(since_last_update, ALLAN) * etemp);
			}
			/* For the PLL the integration interval
			 * (numerator) is the minimum of the update
			 * interval and poll interval. This allows
			 * oversampling, but not undersampling.
			 */
			etemp = MIND(since_last_update, (1 << G.poll_exp));
			dtemp = (4 * PLL) << G.poll_exp;
			freq_drift += offset * etemp / SQUARE(dtemp);
#endif
			set_new_values(STATE_SYNC, offset, recv_time);
			break;
		}
		if (G.stratum != p->lastpkt_stratum + 1) {
			G.stratum = p->lastpkt_stratum + 1;
			run_script("stratum", offset);
		}
	}

	G.reftime = G.cur_time;
	G.ntp_status = p->lastpkt_status;
	G.refid = p->lastpkt_refid;
	G.rootdelay = p->lastpkt_rootdelay + p->lastpkt_delay;
	dtemp = p->filter_jitter; // SQRT(SQUARE(p->filter_jitter) + SQUARE(G.cluster_jitter));
	dtemp += MAXD(p->filter_dispersion + FREQ_TOLERANCE * (G.cur_time - p->lastpkt_recv_time) + abs_offset, MINDISP);
	G.rootdisp = p->lastpkt_rootdisp + dtemp;
	VERB4 bb_error_msg("updating leap/refid/reftime/rootdisp from peer %s", p->p_dotted);

	/* We are in STATE_SYNC now, but did not do adjtimex yet.
	 * (Any other state does not reach this, they all return earlier)
	 * By this time, freq_drift and offset are set
	 * to values suitable for adjtimex.
	 */
#if !USING_KERNEL_PLL_LOOP
	/* Calculate the new frequency drift and frequency stability (wander).
	 * Compute the clock wander as the RMS of exponentially weighted
	 * frequency differences. This is not used directly, but can,
	 * along with the jitter, be a highly useful monitoring and
	 * debugging tool.
	 */
	dtemp = G.discipline_freq_drift + freq_drift;
	G.discipline_freq_drift = MAXD(MIND(MAXDRIFT, dtemp), -MAXDRIFT);
	etemp = SQUARE(G.discipline_wander);
	dtemp = SQUARE(dtemp);
	G.discipline_wander = SQRT(etemp + (dtemp - etemp) / AVG);

	VERB4 bb_error_msg("discipline freq_drift=%.9f(int:%ld corr:%e) wander=%f",
			G.discipline_freq_drift,
			(long)(G.discipline_freq_drift * 65536e6),
			freq_drift,
			G.discipline_wander);
#endif
	VERB4 {
		memset(&tmx, 0, sizeof(tmx));
		if (adjtimex(&tmx) < 0)
			bb_perror_msg_and_die("adjtimex");
		bb_error_msg("p adjtimex freq:%ld offset:%+ld status:0x%x tc:%ld",
				tmx.freq, tmx.offset, tmx.status, tmx.constant);
	}

	memset(&tmx, 0, sizeof(tmx));
#if 0
	tmx.modes = ADJ_FREQUENCY | ADJ_OFFSET;
	/* 65536 is one ppm */
	tmx.freq = G.discipline_freq_drift * 65536e6;
#endif
	tmx.modes = ADJ_OFFSET | ADJ_STATUS | ADJ_TIMECONST;// | ADJ_MAXERROR | ADJ_ESTERROR;
	tmx.constant = (int)G.poll_exp - 4;
	/* EXPERIMENTAL.
	 * The below if statement should be unnecessary, but...
	 * It looks like Linux kernel's PLL is far too gentle in changing
	 * tmx.freq in response to clock offset. Offset keeps growing
	 * and eventually we fall back to smaller poll intervals.
	 * We can make correction more agressive (about x2) by supplying
	 * PLL time constant which is one less than the real one.
	 * To be on a safe side, let's do it only if offset is significantly
	 * larger than jitter.
	 */
	if (G.offset_to_jitter_ratio >= TIMECONST_HACK_GATE)
		tmx.constant--;
	tmx.offset = (long)(offset * 1000000); /* usec */
	if (SLEW_THRESHOLD < STEP_THRESHOLD) {
		if (tmx.offset > (long)(SLEW_THRESHOLD * 1000000)) {
			tmx.offset = (long)(SLEW_THRESHOLD * 1000000);
			tmx.constant--;
		}
		if (tmx.offset < -(long)(SLEW_THRESHOLD * 1000000)) {
			tmx.offset = -(long)(SLEW_THRESHOLD * 1000000);
			tmx.constant--;
		}
	}
	if (tmx.constant < 0)
		tmx.constant = 0;

	tmx.status = STA_PLL;
	if (G.ntp_status & LI_PLUSSEC)
		tmx.status |= STA_INS;
	if (G.ntp_status & LI_MINUSSEC)
		tmx.status |= STA_DEL;

	rc = adjtimex(&tmx);
	if (rc < 0)
		bb_perror_msg_and_die("adjtimex");
	/* NB: here kernel returns constant == G.poll_exp, not == G.poll_exp - 4.
	 * Not sure why. Perhaps it is normal.
	 */
	VERB4 bb_error_msg("adjtimex:%d freq:%ld offset:%+ld status:0x%x",
				rc, tmx.freq, tmx.offset, tmx.status);
	G.kernel_freq_drift = tmx.freq / 65536;
	VERB2 bb_error_msg("update from:%s offset:%+f delay:%f jitter:%f clock drift:%+.3fppm tc:%d",
			p->p_dotted,
			offset,
			p->lastpkt_delay,
			G.discipline_jitter,
			(double)tmx.freq / 65536,
			(int)tmx.constant
	);

	return 1; /* "ok to increase poll interval" */
}
