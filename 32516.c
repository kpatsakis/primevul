void debug_hdr(const char *dir, struct session *s, const char *start, const char *end)
{
	int max;
	chunk_printf(&trash, "%08x:%s.%s[%04x:%04x]: ", s->uniq_id, s->be->id,
		      dir,
		     objt_conn(s->req->prod->end) ? (unsigned short)objt_conn(s->req->prod->end)->t.sock.fd : -1,
		     objt_conn(s->req->cons->end) ? (unsigned short)objt_conn(s->req->cons->end)->t.sock.fd : -1);

	for (max = 0; start + max < end; max++)
		if (start[max] == '\r' || start[max] == '\n')
			break;

	UBOUND(max, trash.size - trash.len - 3);
	trash.len += strlcpy2(trash.str + trash.len, start, max + 1);
	trash.str[trash.len++] = '\n';
	shut_your_big_mouth_gcc(write(1, trash.str, trash.len));
}
