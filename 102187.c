int lsof_main(int argc, char *argv[])
{
 long int pid = 0;
 char* endptr;
 if (argc == 2) {
        pid = strtol(argv[1], &endptr, 10);
 }

    print_header();

 if (pid) {
        lsof_dumpinfo(pid);
 } else {
 DIR *dir = opendir("/proc");
 if (dir == NULL) {
            fprintf(stderr, "Couldn't open /proc\n");
 return -1;
 }

 struct dirent* de;
 while ((de = readdir(dir))) {
 if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
 continue;

            pid = strtol(de->d_name, &endptr, 10);
 if (*endptr != '\0')
 continue;

            lsof_dumpinfo(pid);
 }
        closedir(dir);
 }

 return 0;
}
