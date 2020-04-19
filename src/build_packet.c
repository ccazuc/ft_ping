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

	size = (ICMP_MINLEN + env->payload_len) / 2;
	i = -1;
	sum = 0;
	packet = (uint16_t*)&env->packet->icmp_header;
	while (++i < size)
	{
		sum += packet[i];
		//printf("packet[%d] = %x\n", i, packet[i]);
	}
	if ((uint32_t)(i * 2) < ICMP_MINLEN + env->payload_len)
		sum += packet[i] & 0x00FF;
	//if (i < size)
	//	sum += packet[i];
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
	env->packet->icmp_header.icmp_hun.ih_idseq.icd_id = getpid();
	//env->packet->icmp_header.icmp_hun.ih_idseq.icd_id = 0x1234;
	env->packet->icmp_header.icmp_hun.ih_idseq.icd_seq = env->count;
	//env->packet->icmp_header.icmp_hun.ih_idseq.icd_seq = 0x4567;
	for (uint8_t i = 0; i < 8; ++i)
		printf("hdr[%d] = %x\n", i, ((unsigned char*)&env->packet->icmp_header)[i]);
	build_icmp_checksum(env);
}

void build_packet(t_env *env)
{
	for (uint8_t i = env->packet_len - env->payload_len; i < env->packet_len - 1; ++i)
		((unsigned char*)env->packet)[i] = 2;
	((unsigned char*)env->packet)[env->packet_len - 1] = 3;
	build_ip_header(env);
	build_icmp_header(env);
}
