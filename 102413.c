static int is_hex(char c) {
 if (((c >= '0') && (c <= '9')) ||
 ((c >= 'a') && (c <= 'f')) ||
 ((c >= 'A') && (c <= 'F')))
 return(1);
 return(0);
}
