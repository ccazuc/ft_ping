#include "ft_ping.h"

void send_ping(t_env *env)
{
	int32_t sent;

	for (uint8_t i = 0; i < 14; ++i)
		printf("%d", env->sockaddr->sa_data[i]);
	printf("\n");
	unsigned char *ptr = (unsigned char*)env->packet;
	for (uint8_t i = 0; i < env->packet_len; ++i)
		printf("%x ", ptr[i]);
	printf("\n");
	if ((sent = sendto(env->fd, env->packet, env->packet_len, 0, env->sockaddr, env->addrlen)) == -1)
		ft_exit("sendto failed, exiting.", EXIT_FAILURE);
	printf("sent %d to %s, addrlen %d, packetlen %d\n", sent, env->addr, env->addrlen, env->packet_len);
}
