#include "ft_ping.h"

void init_connect(t_env *env)
{
	int32_t osef;

	if ((env->fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
		ft_exit("Error, could not create socket, exiting.", EXIT_FAILURE);
	osef = 1;
	if (setsockopt(env->fd, IPPROTO_IP, IP_HDRINCL, &osef, sizeof(osef)) == -1)
	{
		ft_exit("Could not set socket options, exiting.", EXIT_FAILURE);
	}
}
