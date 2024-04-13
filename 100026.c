static void UpdateRTInfo(const char *legend)
{
	GF_SystemRTInfo rti;

	/*refresh every second*/
	if (!display_rti && !rti_logs) return;
	if (!gf_sys_get_rti(rti_update_time_ms, &rti, 0) && !legend)
		return;

	if (display_rti) {
		char szMsg[1024];

		if (rti.total_cpu_usage && (bench_mode<2) ) {
			sprintf(szMsg, "FPS %02.02f CPU %2d (%02d) Mem %d kB",
			        gf_term_get_framerate(term, 0), rti.total_cpu_usage, rti.process_cpu_usage, (u32) (rti.gpac_memory / 1024));
		} else {
			sprintf(szMsg, "FPS %02.02f CPU %02d Mem %d kB",
			        gf_term_get_framerate(term, 0), rti.process_cpu_usage, (u32) (rti.gpac_memory / 1024) );
		}

		if (display_rti==2) {
			if (bench_mode>=2) {
				PrintAVInfo(GF_FALSE);
			}
			fprintf(stderr, "%s\r", szMsg);
		} else {
			GF_Event evt;
			evt.type = GF_EVENT_SET_CAPTION;
			evt.caption.caption = szMsg;
			gf_term_user_event(term, &evt);
		}
	}
	if (rti_logs) {
		fprintf(rti_logs, "% 8d\t% 8d\t% 8d\t% 4d\t% 8d\t%s",
		        gf_sys_clock(),
		        gf_term_get_time_in_ms(term),
		        rti.total_cpu_usage,
		        (u32) gf_term_get_framerate(term, 0),
		        (u32) (rti.gpac_memory / 1024),
		        legend ? legend : ""
		       );
		if (!legend) fprintf(rti_logs, "\n");
	}
}
