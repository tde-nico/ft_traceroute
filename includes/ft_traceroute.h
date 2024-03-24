#ifndef FT_TRACEROUT_H
# define FT_TRACEROUT_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <sys/time.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>


typedef struct	s_traceroute
{
	int				resolve;
	int				max_hop;
	u_int8_t		min_hop;
	u_int8_t		num_pkts;
	struct timeval	wait;
	int				debug;
	char			*hostname;
	int				size;

	struct addrinfo	dest_info;
	struct sockaddr dest_sockaddr;
	struct in_addr	dest_addr;

	char			ip[INET_ADDRSTRLEN];
	char			addr[NI_MAXHOST];

	int				sockfd;

	u_int32_t		last_resolved;

	struct timeval	probe_time;
	struct timeval	recv_time;

}	t_traceroute;


// debug.c
void			debug(t_traceroute *t);

// libft.c
size_t			ft_strlen(const char *str);
int				ft_strncmp(const char *s1, const char *s2, unsigned int n);
void			*ft_memset(void *b, int c, size_t len);
int				ft_atoi(const char *str);

// parse.c
int				arg_parse(t_traceroute *t, size_t argc, char **argv);

// recv.c
int				recv_probes(t_traceroute *t);

// send.c
int				send_probes(t_traceroute *t);

// setup.c
int				setup(t_traceroute *t);

// time.c
struct timeval	get_time(void);
struct timeval	secs2time(double secs);


#endif
