#include <stdio.h>
#include <arpa/inet.h>


int main() {
    unsigned short host_short = 0x1234;
    unsigned long host_long = 0x12345678;

    // Conversion de l'entier court de l'hôte à l'ordre de bytes réseau
    unsigned short network_short = htons(host_short);
    printf("Host Short: 0x%04X, Network Short: 0x%04X\n", host_short, network_short);

    // Conversion de l'entier long de l'hôte à l'ordre de bytes réseau
    unsigned long network_long = htonl(host_long);
    printf("Host Long: 0x%08lX, Network Long: 0x%08lX\n", host_long, network_long);

    // Conversion de l'entier court du réseau à l'ordre de bytes de l'hôte
    unsigned short host_short_new = ntohs(network_short);
    printf("Network Short: 0x%04X, Host Short New: 0x%04X\n", network_short, host_short_new);

    // Conversion de l'entier long du réseau à l'ordre de bytes de l'hôte
    unsigned long host_long_new = ntohl(network_long);
    printf("Network Long: 0x%08lX, Host Long New: 0x%08lX\n", network_long, host_long_new);
	htons(8080);

    return 0;
}
