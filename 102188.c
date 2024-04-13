static void print_fds(struct pid_info_t* info)
{
 static char* fd_path = "fd/";
    strlcat(info->path, fd_path, sizeof(info->path));

 int previous_length = info->parent_length;
    info->parent_length += strlen(fd_path);

 DIR *dir = opendir(info->path);
 if (dir == NULL) {
 char msg[BUF_MAX];
        snprintf(msg, sizeof(msg), "%s (opendir: %s)", info->path, strerror(errno));
        printf("%-9s %5d %10s %4s %9s %18s %9s %10s %s\n",
                info->cmdline, info->pid, info->user, "FDS",
 "", "", "", "", msg);
 goto out;
 }

 struct dirent* de;
 while ((de = readdir(dir))) {
 if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
 continue;

        print_type(de->d_name, info);
 }
    closedir(dir);

out:
    info->parent_length = previous_length;
    info->path[info->parent_length] = '\0';
}
