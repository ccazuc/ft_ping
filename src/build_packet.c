#include "ft_ping.h"

static void build_ip_checksum(t_env *env)
{
	int32_t size = 0;
	uint32_t sum;
	int32_t i;
	uint16_t *packet;

	size = sizeof(env->packet->ip_header);
	i = -1;
	sum = 0;
	packet = (uint16_t*)&env->packet->ip_header;
	while (++i < size - 1)
	{
		sum += packet[i];
		++i;
	}
	if (i < size)
		sum += packet[i] & 0xFF00;
	while (sum > 0xFFFF)
		sum = (sum & 0xFFFF) + (sum >> 16);
	env->packet->ip_header.ip_sum = ~sum;
}

static void build_ip_header(t_env *env)
{
	env->packet->ip_header.ip_v = 4;
	env->packet->ip_header.ip_hl = 5;
	env->packet->ip_header.ip_tos = 0;
	env->packet->ip_header.ip_len = sizeof(env->packet->ip_header) + sizeof(env->packet->icmp_header) + env->payload_len;
	env->packet->ip_header.ip_id = 0;
	env->packet->ip_header.ip_off = IP_DF;
	env->packet->ip_header.ip_ttl = 0xFF;
	env->packet->ip_header.ip_p = IPPROTO_ICMP;
	env->packet->ip_header.ip_sum = 0;
	env->packet->ip_header.ip_src.s_addr = 0;
	int result;
	if ((result = inet_pton(AF_INET, "127.0.0.1", &env->packet->ip_header.ip_src)) != 1)
	{
		if (!result)
			printf("Format error for src\n");
		else
			perror("inet_pton");
		ft_exit("Failed to convert src ip as binary, exiting.", EXIT_FAILURE);
	}
	if ((result = inet_pton(AF_INET, env->addr, &env->packet->ip_header.ip_dst)) != 1)
	{
		if (!result)
			printf("Format error for dst\n");
		else
			perror("inet_pton");
		ft_exit("Failed to convert dst ip as binary, exiting.", EXIT_FAILURE);
	}
	build_ip_checksum(env);
}

static void build_icmp_checksum(t_env *env)
{
	int32_t size = 0;
	uint32_t sum;
	int32_t i;
	uint16_t *packet;

	size = sizeof(env->packet->icmp_header);
	i = -1;
	sum = 0;
	packet = (uint16_t*)&env->packet->icmp_header;
	while (++i < size - 1)
	{
		sum += packet[i];
		++i;
	}
	if (i < size)
		sum += packet[i] & 0xFF00;
	while (sum > 0xFFFF)
		sum = (sum & 0xFFFF) + (sum >> 16);
	env->packet->icmp_header.icmp_cksum = ~sum;
}

static void build_icmp_header(t_env *env)
{
	env->packet->icmp_header.icmp_type = ICMP_ECHO;
	env->packet->icmp_header.icmp_code = 0;
	env->packet->icmp_header.icmp_cksum = 0;
	env->packet->icmp_header.icmp_hun.ih_idseq.icd_id = 0x1234;
	//env->packet->icmp_header.icmp_hun.ih_idseq.icd_seq = env->count;
	env->packet->icmp_header.icmp_hun.ih_idseq.icd_seq = 0x4567;
	build_icmp_checksum(env);
}

void build_packet(t_env *env)
{
	build_ip_header(env);
	build_icmp_header(env);
}
