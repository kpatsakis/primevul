static const char *ep_state_name(uint32_t state)
{
    return lookup_name(state, ep_state_names,
                       ARRAY_SIZE(ep_state_names));
}
