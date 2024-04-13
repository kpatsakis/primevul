barrier_is_horizontal(const struct PointerBarrier *barrier)
{
    return barrier->y1 == barrier->y2;
}
