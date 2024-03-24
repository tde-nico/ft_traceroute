#include "ft_traceroute.h"

int is_probe(u_int8_t *pkt)
{
	struct iphdr	*ip_hdr;
	struct icmphdr	*icmp_hdr;
	struct iphdr	*in_ip_hdr;
	struct icmphdr	*in_icmp_hdr;

	ip_hdr = (struct iphdr *)pkt;
	icmp_hdr = (struct icmphdr *)(pkt + (ip_hdr->ihl << 2));
	if (icmp_hdr->type == ICMP_TIME_EXCEEDED)
	{
		in_ip_hdr = (struct iphdr *)((void *)icmp_hdr + sizeof(struct icmphdr));
		in_icmp_hdr = (struct icmphdr *)((void *)in_ip_hdr + (in_ip_hdr->ihl << 2));
		if (in_icmp_hdr->type == ICMP_ECHO &&
			in_icmp_hdr->un.echo.id == htons((uint16_t)getpid()))
			return (1);
	}
	else if (icmp_hdr->type == ICMP_ECHOREPLY &&
		icmp_hdr->un.echo.id == htons((uint16_t)getpid()))
		return (1);
	return (0);
}

int	print_info(t_traceroute *t, struct sockaddr	*sender_addr)
{
	struct sockaddr_in	*addr;
	struct sockaddr_in	res_addr;
	char				ip[INET_ADDRSTRLEN];
	char				resolved[NI_MAXHOST];
	int					ret;

	addr = (struct sockaddr_in *)sender_addr;
	if (addr->sin_addr.s_addr != t->last_resolved)
	{
		t->last_resolved = (struct in_addr){addr->sin_addr.s_addr}.s_addr;
		res_addr.sin_addr = (struct in_addr){addr->sin_addr.s_addr};
		res_addr.sin_family = AF_INET;
		inet_ntop(res_addr.sin_family,
			&res_addr.sin_addr, ip, sizeof(ip));
		printf("  %s", ip);
		if (t->resolve)
		{
			ret = getnameinfo((struct sockaddr *)&res_addr,
				sizeof(res_addr), resolved, sizeof(resolved), NULL, 0, 0);
			if (ret < 0)
				return (printf("Error on getnameinfo: %s\n", gai_strerror(ret)));
			printf(" (%s)", resolved);
		}
	}
	printf("  %.3f ms", (t->recv_time.tv_sec - t->probe_time.tv_sec) * 1000.0f
		+ (t->recv_time.tv_usec - t->probe_time.tv_usec) / 1000.0f);
	return (0);
}

int	recv_probes(t_traceroute *t)
{
	u_int8_t		pkt[IP_MAXPACKET];
	struct sockaddr	sender_addr;
	struct timeval	timeout;
	socklen_t		addr_len;
	fd_set			readfds;
	int				ret;
	int				i;

	addr_len = sizeof(sender_addr);
	printf("%*d", 2, t->min_hop);
	i = -1;
	while (++i < t->num_pkts)
	{
		FD_ZERO(&readfds);
		FD_SET(t->sockfd, &readfds);
		timeout = t->wait;
		ret = select(t->sockfd + 1, &readfds, NULL, NULL, &timeout);
		if (ret)
		{
			ret = recvfrom(t->sockfd, pkt, IP_MAXPACKET,
				0, &sender_addr, &addr_len);
			if (ret > 0)
			{
				if (is_probe(pkt))
				{
					t->recv_time = get_time();
					if (print_info(t, &sender_addr))
						return (1);
					continue ;
				}
			}
			else if (ret < 0)
				return (printf("Error on recvfrom: %s\n", strerror(errno)));
		}
		printf("  *");
	}
	printf("\n");
	return (0);
}
