#include "ft_ping.h"

int32_t resolve_host(t_env *env, char *dst)
{
	struct addrinfo hints;
	struct addrinfo *result;
	char *ip;

	env->dst = dst;
	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	if (getaddrinfo(dst, NULL, &hints, &result) || !result)
		print_unknown_dst(dst);
	if (!(ip = malloc(INET_ADDRSTRLEN)))
		ft_exit("Error, malloc for ip failed", EXIT_FAILURE);
	ft_memset(ip, 0, INET_ADDRSTRLEN);
	if (!inet_ntop(AF_INET, &((struct sockaddr_in*)result->ai_addr)->sin_addr, ip, INET_ADDRSTRLEN))
		print_unknown_dst(dst);
	env->addr = ip;
	//if (!(env->sockaddr = malloc(result->ai_addrlen)))
	//	ft_exit("Error, malloc for sockaddr failed", EXIT_FAILURE);
	//memcpy(env->sockaddr, result->ai_addr, result->ai_addrlen);
	if (!(env->sockaddr = malloc(sizeof(*env->sockaddr))))
		ft_exit("Error, malloc for sockaddr failed", EXIT_FAILURE);
	memset(env->sockaddr, 0, sizeof(*env->sockaddr));
	memcpy(env->sockaddr, result->ai_addr, result->ai_addrlen);
	env->addrlen = result->ai_addrlen;
	free (result);
	return 0;
}
