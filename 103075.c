static bool ga_create_file(const char *path)
{
    int fd = open(path, O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR);
    if (fd == -1) {
        g_warning("unable to open/create file %s: %s", path, strerror(errno));
        return false;
    }
    close(fd);
    return true;
}
