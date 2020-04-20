#include "ft_ping.h"

static void build_datas(t_env *env)
{
	env->send_packet->icmp_header.icmp_hun.ih_idseq.icd_seq = env->count;
	env->send_packet->icmp_header.icmp_cksum = 0;
	if (env->payload_len >= sizeof(size_t))
	{
		size_t time = get_time();
		memcpy(((unsigned char*)env->send_packet) + env->packet_len - env->payload_len, &time, sizeof(time));
	}
	build_icmp_checksum(env);
}

void send_ping(t_env *env)
{
	int32_t sent;

	++env->count;
	build_datas(env);
	if ((sent = sendto(env->fd, env->send_packet, env->packet_len, 0, env->sockaddr, env->addrlen)) == -1)
		ft_exit("sendto failed, exiting.", EXIT_FAILURE);
	++env->ping_sent;
	//printf("sent %d to %s, addrlen %d, packetlen %d\n", sent, env->addr, env->addrlen, env->packet_len);
}
