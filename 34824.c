barrier_is_blocking_direction(const struct PointerBarrier * barrier,
                              int direction)
{
    /* Barriers define which way is ok, not which way is blocking */
    return (barrier->directions & direction) != direction;
}
