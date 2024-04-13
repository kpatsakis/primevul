static int noninteractive(int argc, char **argv) {
    int retval = 0;
    if (config.stdinarg) {
        argv = zrealloc(argv, (argc+1)*sizeof(char*));
        argv[argc] = readArgFromStdin();
        retval = issueCommand(argc+1, argv);
    } else {
        retval = issueCommand(argc, argv);
    }
    return retval;
}
