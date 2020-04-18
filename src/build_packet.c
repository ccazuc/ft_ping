#include "ft_ping.h"

static void build_ip_header(t_env *env)
{
	env->packet->ip_header.ip_v = 4;
	env->packet->ip_header.ip_hl = 5;
	env->packet->ip_header.ip_tos = 0;
	env->packet->ip_header.ip_id = 0;
	env->packet->ip_header.ip_off = ntohs(IP_DF);
	env->packet->ip_header.ip_ttl = 0xFF;
	env->packet->ip_header.ip_p = IPPROTO_ICMP;
	env->packet->ip_header.ip_sum = 0;
	int result;
	env->packet->ip_header.ip_src.s_addr = 0;
	if ((result = inet_pton(AF_INET, env->addr, &env->packet->ip_header.ip_dst)) != 1)
	{
		if (!result)
			printf("Format error for dst\n");
		else
			perror("inet_pton");
		ft_exit("Failed to convert dst ip as binary, exiting.", EXIT_FAILURE);
	}
}

static void build_icmp_checksum(t_env *env)
{
	int32_t size = 0;
	uint32_t sum;
	int32_t i;
	uint16_t *packet;

	size = sizeof(env->packet->icmp_header) / 2;
	i = -1;
	sum = 0;
	packet = (uint16_t*)&env->packet->icmp_header;
	while (++i < size)
		sum += packet[i];
	if (i < size)
		sum += packet[i] & 0x00FF;
	while (sum > 0xFFFF)
		sum = (sum & 0xFFFF) + (sum >> 16);
	env->packet->icmp_header.icmp_cksum = ~sum & 0xFFFF;
	printf("icmp_header_checksum %x\n", ~sum & 0xFFFF);
}

static void build_icmp_header(t_env *env)
{
	env->packet->icmp_header.icmp_type = ICMP_ECHO;
	env->packet->icmp_header.icmp_code = 0;
	env->packet->icmp_header.icmp_cksum = 0;
	env->packet->icmp_header.icmp_hun.ih_idseq.icd_id = env->count;
	env->packet->icmp_header.icmp_hun.ih_idseq.icd_seq = env->count;
	build_icmp_checksum(env);
}

void build_packet(t_env *env)
{
	build_ip_header(env);
	build_icmp_header(env);
}
