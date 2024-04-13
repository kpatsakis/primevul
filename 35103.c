read_event (struct EventFixture *ef) {
    int r = read(ef->test_sock, ef->event_buffer, 1023); \
    ef->event_buffer[r] = 0;
}
