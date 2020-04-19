#include "ft_ping.h"

void print_usage(int32_t code)
{
	printf("Usage: ping [-vh] [-c count] [-s packetsize] destination\n");
	exit(code);
}

void print_invalid_param(uint8_t param)
{
	printf("ping : option invalide -- \'%c\'\n", param);
	print_usage(EXIT_FAILURE);
}

void print_unknown_dst(char *dst)
{
	printf("ping: %s: Nom ou service inconnu\n", dst);
	exit(EXIT_FAILURE);
}
