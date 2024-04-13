void UseMsg(void)
{
    ErrorF("use: X [:<display>] [option]\n");
    ErrorF("-a #                   default pointer acceleration (factor)\n");
    ErrorF("-ac                    disable access control restrictions\n");
    ErrorF("-audit int             set audit trail level\n");	
    ErrorF("-auth file             select authorization file\n");	
    ErrorF("-br                    create root window with black background\n");
    ErrorF("+bs                    enable any backing store support\n");
    ErrorF("-bs                    disable any backing store support\n");
    ErrorF("-c                     turns off key-click\n");
    ErrorF("c #                    key-click volume (0-100)\n");
    ErrorF("-cc int                default color visual class\n");
    ErrorF("-nocursor              disable the cursor\n");
    ErrorF("-core                  generate core dump on fatal error\n");
    ErrorF("-dpi int               screen resolution in dots per inch\n");
#ifdef DPMSExtension
    ErrorF("-dpms                  disables VESA DPMS monitor control\n");
#endif
    ErrorF("-deferglyphs [none|all|16] defer loading of [no|all|16-bit] glyphs\n");
    ErrorF("-f #                   bell base (0-100)\n");
    ErrorF("-fc string             cursor font\n");
    ErrorF("-fn string             default font name\n");
    ErrorF("-fp string             default font path\n");
    ErrorF("-help                  prints message with these options\n");
    ErrorF("-I                     ignore all remaining arguments\n");
#ifdef RLIMIT_DATA
    ErrorF("-ld int                limit data space to N Kb\n");
#endif
#ifdef RLIMIT_NOFILE
    ErrorF("-lf int                limit number of open files to N\n");
#endif
#ifdef RLIMIT_STACK
    ErrorF("-ls int                limit stack space to N Kb\n");
#endif
    ErrorF("-nolock                disable the locking mechanism\n");
    ErrorF("-nolisten string       don't listen on protocol\n");
    ErrorF("-noreset               don't reset after last client exists\n");
    ErrorF("-background [none]     create root window with no background\n");
    ErrorF("-reset                 reset after last client exists\n");
    ErrorF("-p #                   screen-saver pattern duration (minutes)\n");
    ErrorF("-pn                    accept failure to listen on all ports\n");
    ErrorF("-nopn                  reject failure to listen on all ports\n");
    ErrorF("-r                     turns off auto-repeat\n");
    ErrorF("r                      turns on auto-repeat \n");
    ErrorF("-render [default|mono|gray|color] set render color alloc policy\n");
    ErrorF("-retro                 start with classic stipple and cursor\n");
    ErrorF("-s #                   screen-saver timeout (minutes)\n");
    ErrorF("-seat string           seat to run on\n");
    ErrorF("-t #                   default pointer threshold (pixels/t)\n");
    ErrorF("-terminate             terminate at server reset\n");
    ErrorF("-to #                  connection time out\n");
    ErrorF("-tst                   disable testing extensions\n");
    ErrorF("ttyxx                  server started from init on /dev/ttyxx\n");
    ErrorF("v                      video blanking for screen-saver\n");
    ErrorF("-v                     screen-saver without video blanking\n");
    ErrorF("-wm                    WhenMapped default backing-store\n");
    ErrorF("-wr                    create root window with white background\n");
    ErrorF("-maxbigreqsize         set maximal bigrequest size \n");
#ifdef PANORAMIX
    ErrorF("+xinerama              Enable XINERAMA extension\n");
    ErrorF("-xinerama              Disable XINERAMA extension\n");
#endif
    ErrorF("-dumbSched             Disable smart scheduling, enable old behavior\n");
    ErrorF("-schedInterval int     Set scheduler interval in msec\n");
    ErrorF("-sigstop               Enable SIGSTOP based startup\n");
    ErrorF("+extension name        Enable extension\n");
    ErrorF("-extension name        Disable extension\n");
#ifdef XDMCP
    XdmcpUseMsg();
#endif
    XkbUseMsg();
    ddxUseMsg();
}
