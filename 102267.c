get_option_uint8(uint8_t *i, const struct dhcp_message *dhcp, uint8_t option)
{
 const uint8_t *p = get_option_raw(dhcp, option);

 if (!p)
 return -1;
 if (i)
 *i = *(p);
 return 0;
}
