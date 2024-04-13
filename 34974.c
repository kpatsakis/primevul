delay_q_take()
{
    job j = delay_q_peek();
    return j ? pq_take(&j->tube->delay) : NULL;
}
