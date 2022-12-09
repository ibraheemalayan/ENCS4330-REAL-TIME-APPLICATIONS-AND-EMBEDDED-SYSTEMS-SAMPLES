#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

main(void)
{
  struct hostent *host;
  static char who[10];

  printf("Enter host name to look up: ");
  scanf("%10s", who);
  
  host = gethostbyname(who);
    
  if ( host != (struct hostent *) NULL ) {
    printf("Here is what I found about %s :\n", who);
    
    printf("Official name: %s\n", host->h_name);
    printf("Aliases      : ");
    while ( *host->h_aliases ) {
      printf("%s  ", *host->h_aliases);
      ++host->h_aliases;
    }
    printf("\nAddress type   : %i\n", host->h_addrtype);
    printf("Address length: %i\n", host->h_length);
    printf("Address list  :");

    while( *host->h_addr_list ) {
      struct in_addr in;
      memcpy(&in.s_addr, *host->h_addr_list, sizeof(in.s_addr));
      printf("[%s] = %s  ", *host->h_addr_list, inet_ntoa(in));
      ++host->h_addr_list;
    }
    printf("\n");
  }
}
