wait_for_gdb()
{
    if (!waitforgdb) {
        return;
    }


    std::cout << std::endl << "  Attach GDB to PID " << getpid()
              << " to debug!" << std::endl
              << "  This thread will block until then!" << std::endl
              << "  Once blocked here, you can set other breakpoints."
              << std::endl
              << "  Do a \"set variable waitforgdb=$false\" to continue"
              << std::endl << std::endl;

    while (waitforgdb) {
        sleep(1);
    }
}
