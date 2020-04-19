#include "ft_ping.h"

void receive_ping(t_env *env)
{
	struct msghdr msg;
	//struct cmsghdr *cmsg;
	struct iovec iovec;
	ssize_t received;
	//int receive_ttl;
	char ip[16];
	t_packet *packet;

	memset(&msg, 0, sizeof(msg));
	memset(&iovec, 0, sizeof(iovec));
	if (!(packet = malloc(sizeof(env->packet->ip_header) + sizeof(env->packet->icmp_header) + env->payload_len)))
		ft_exit("Error, could not malloc packet", EXIT_FAILURE);
	iovec.iov_base = packet;
	iovec.iov_len = sizeof(env->packet->ip_header) + sizeof(env->packet->icmp_header) + env->payload_len;
	msg.msg_control = malloc(30);
	msg.msg_controllen = 30;
	memset(msg.msg_control, 1, 30);
	msg.msg_name = env->sockaddr;
	msg.msg_namelen = env->addrlen;
	msg.msg_iov = &iovec;
	msg.msg_iovlen = 1;
	if ((received = recvmsg(env->fd, &msg, 0)) <= 0)
	{
		free(packet);
		if (!received)
			return;
		ft_exit("Error, recvmsg failed, exiting.", EXIT_FAILURE);
	}
	printf("received: %ld\n", received);
	printf("ip_version: %d | ip_hl %d | ip_ttl %d | ip_tos %d | ip_id %d | ip_off 0x%x | ip_sum 0x%x | ip_src_addr 0x%x | ip_dst_addr 0x%x\n", packet->ip_header.ip_v, packet->ip_header.ip_hl, packet->ip_header.ip_ttl, packet->ip_header.ip_tos, packet->ip_header.ip_id, packet->ip_header.ip_off, packet->ip_header.ip_sum, htonl(*((uint32_t*)&packet->ip_header.ip_src)), htonl(*((uint32_t*)&packet->ip_header.ip_dst))); 
	printf("icmp_type %d | icmp_code %d | icmp_cksum %x | icmp_id 0x%x | icmp_seq 0x%x\n", packet->icmp_header.icmp_type, packet->icmp_header.icmp_code, packet->icmp_header.icmp_cksum, packet->icmp_header.icmp_hun.ih_idseq.icd_id, packet->icmp_header.icmp_hun.ih_idseq.icd_seq);
	if (packet->icmp_header.icmp_type || packet->icmp_header.icmp_code || packet->icmp_header.icmp_hun.ih_idseq.icd_id != getpid())
		printf("Invalid packet\n");
	printf("%ld bytes from %s (%s): icmp_seq=%d ttl=%d\n", received - sizeof(packet->ip_header), "", "", packet->icmp_header.icmp_hun.ih_idseq.icd_seq, packet->ip_header.ip_ttl);
	free(packet);
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
