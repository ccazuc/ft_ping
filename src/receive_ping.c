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
	//iovec.iov_len = env->packet_len;
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
	//printf("received: %ld\n", received);
	//printf("ip_version: %d | ip_hl %d | ip_ttl %d | ip_tos %d | ip_id %d | ip_off 0x%x | ip_sum 0x%x | ip_src_addr 0x%x | ip_dst_addr 0x%x\n", env->receive_packet->ip_header.ip_v, env->receive_packet->ip_header.ip_hl, env->receive_packet->ip_header.ip_ttl, env->receive_packet->ip_header.ip_tos, env->receive_packet->ip_header.ip_id, env->receive_packet->ip_header.ip_off, env->receive_packet->ip_header.ip_sum, htonl(*((uint32_t*)&env->receive_packet->ip_header.ip_src)), htonl(*((uint32_t*)&env->receive_packet->ip_header.ip_dst))); 
	//printf("icmp_type %d | icmp_code %d | icmp_cksum %x | icmp_id 0x%x | icmp_seq 0x%x\n", env->receive_packet->icmp_header.icmp_type, env->receive_packet->icmp_header.icmp_code, env->receive_packet->icmp_header.icmp_cksum, env->receive_packet->icmp_header.icmp_hun.ih_idseq.icd_id, env->receive_packet->icmp_header.icmp_hun.ih_idseq.icd_seq);
	if (env->receive_packet->icmp_header.icmp_type || env->receive_packet->icmp_header.icmp_code || env->receive_packet->icmp_header.icmp_hun.ih_idseq.icd_id != getpid() || env->receive_packet->icmp_header.icmp_hun.ih_idseq.icd_seq != env->count)
	{
		return;
	}
	++env->ping_received;
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
		printf("%ld bytes from %s (%s): icmp_seq=%d ttl=%d time=%.1f ms\n", received - sizeof(env->receive_packet->ip_header), env->dst, env->addr, env->receive_packet->icmp_header.icmp_hun.ih_idseq.icd_seq, env->receive_packet->ip_header.ip_ttl, diff / 1000.f);
	}
	else
	{
		printf("%ld bytes from %s (%s): icmp_seq=%d ttl=%d\n", received - sizeof(env->receive_packet->ip_header), "", "", env->receive_packet->icmp_header.icmp_hun.ih_idseq.icd_seq, env->receive_packet->ip_header.ip_ttl);
	}
	/*for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL;	cmsg = CMSG_NXTHDR(&msg, cmsg))
	{
		printf("level %d, type %d\n", cmsg->cmsg_level, cmsg->cmsg_type);
		if (cmsg->cmsg_level == IPPROTO_IP && cmsg->cmsg_type == IP_TTL)
		{
			memcpy(&receive_ttl, CMSG_DATA(cmsg), sizeof(int));
			printf("receive ttl %d\n", receive_ttl);
			break;
		}
	}*/
}
