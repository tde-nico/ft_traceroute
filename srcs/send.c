#include "ft_traceroute.h"

u_int16_t	ft_cksum(u_int16_t *addr, int len)
{
	int			count;
	u_int32_t	sum;

	count = len;
	sum = 0;
	while (count > 1)
	{
		sum += *(addr++);
		count -= 2;
	}
	if (count)
		sum += *(u_int8_t *)addr;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return ((u_int16_t)~sum);
}

int	send_probes(t_traceroute *t)
{
	u_int8_t		pkt[IP_MAXPACKET];
	struct icmphdr	*icmp_hdr;
	int				ret;
	int				i;

	icmp_hdr = (struct icmphdr *)pkt;
	i = -1;
	while (++i < t->num_pkts)
	{
		ft_memset(pkt, 0, sizeof(pkt));
		icmp_hdr->type = ICMP_ECHO;
		icmp_hdr->un.echo.id = htons((u_int16_t)getpid());
		icmp_hdr->checksum = ft_cksum((u_int16_t *)pkt, t->size);
		ret = sendto(t->sockfd, pkt, t->size, 0,
			&t->dest_sockaddr, t->dest_info.ai_addrlen);
		if (ret < 0)
			return (printf("Error on sendto: %s\n", strerror(errno)));
	}
	t->probe_time = get_time();
	return (0);
}
