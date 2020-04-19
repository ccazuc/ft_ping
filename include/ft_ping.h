#ifndef FT_PING_H
# define FT_PING_H

# include "../libft/includes/libft.h"
# include <stdlib.h>
# include <stdint.h>
# include <stdio.h>
# include <netinet/ip_icmp.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <signal.h>
# include <sys/time.h>

typedef struct s_params
{
	uint8_t v;
} t_params;

typedef struct s_packet
{
	struct ip ip_header;
	struct icmp icmp_header;
} t_packet;

typedef struct s_env
{
	char *dst;
	char *addr;
	struct sockaddr *sockaddr;
	int32_t fd;
	uint8_t addrlen;
	uint16_t count;
	uint32_t payload_len;
	uint32_t packet_len;
	uint32_t ip_header_len;
	uint32_t icmp_header_len;
	uint32_t headers_len;
	uint8_t receiving;
	t_params params;
	t_packet *send_packet;
	t_packet *receive_packet;
	size_t ping_min;
	size_t ping_max;
	size_t ping_total;
	size_t ping_sent;
	size_t ping_received;
	size_t start_time;
	
} t_env;

t_env *init_env(void);
void parse_args(t_env *env, int argc, char **argv);
void print_usage(int32_t code);
void print_unknown_dst(char *dst);
void print_invalid_param(uint8_t param);
int32_t resolve_host(t_env *env, char *dst);
void init_connect(t_env *env);
int32_t sighandler(int32_t signal, void *ptr);
void build_packet(t_env *env);
void send_ping(t_env *env);
void receive_ping(t_env *env);
void build_icmp_checksum(t_env *env);
size_t get_time(void);

#endif
