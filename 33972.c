close_fds(const int (& except)[N])
{
    int numfailed = 0, closed = 0;
    for (int anfd = fileno(stderr)+1; numfailed < 10; anfd++) {
        if (std::find(except, except+N, anfd) != except+N) {
            continue;
        }
        if (close(anfd) < 0) {
            numfailed++;
        } else {
            numfailed = 0;
            closed++;
        }
    }
    gnash::log_debug("Closed %d files.", closed);
} 
