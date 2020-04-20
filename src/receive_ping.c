#include "ft_ping.h"

void receive_ping(t_env *env)
{
	struct msghdr msg;
	struct iovec iovec;
	ssize_t received;

	memset(&msg, 0, sizeof(msg));
	memset(&iovec, 0, sizeof(iovec));
	iovec.iov_base = env->receive_packet;
	iovec.iov_len = sizeof(env->send_packet->ip_header) + sizeof(env->send_packet->icmp_header) + env->payload_len;
	msg.msg_name = env->sockaddr;
	msg.msg_namelen = env->addrlen;
	msg.msg_iov = &iovec;
	msg.msg_iovlen = 1;
	if ((received = recvmsg(env->fd, &msg, 0)) <= 0)
	{
		if (!received)
			return;
		ft_exit("Error, recvmsg failed, exiting.", EXIT_FAILURE);
	}
	size_t gettime = get_time();
	if (env->receive_packet->icmp_header.icmp_type || env->receive_packet->icmp_header.icmp_code || env->receive_packet->icmp_header.icmp_hun.ih_idseq.icd_id != getpid() || env->receive_packet->icmp_header.icmp_hun.ih_idseq.icd_seq != env->count)
	{
		return;
	}
	++env->ping_received;
	printf("%ld bytes from %s (%s): icmp_seq=%d ttl=%d", received - sizeof(env->receive_packet->ip_header), env->addr_name ? env->addr_name : env->dst, env->addr, env->receive_packet->icmp_header.icmp_hun.ih_idseq.icd_seq, env->receive_packet->ip_header.ip_ttl);
	if (((uint32_t) received - env->headers_len) >= sizeof(size_t))
	{
		size_t time = 0;
		memcpy(&time, ((unsigned char*)env->receive_packet) + env->packet_len - env->payload_len, sizeof(time));
		size_t diff = gettime - time;
		if (diff < env->ping_min)
			env->ping_min = diff;
		if (diff > env->ping_max)
			env->ping_max = diff;
		env->ping_total += diff;
		printf(" time=%.1f ms", diff / 1000.f);
	}
	if (received - env->ip_header_len != env->payload_len + env->icmp_header_len)
		printf(" (truncated)");
	printf("\n");
}
