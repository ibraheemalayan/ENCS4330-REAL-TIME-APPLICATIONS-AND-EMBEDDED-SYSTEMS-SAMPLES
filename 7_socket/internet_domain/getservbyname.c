#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>

main(void)
{
  struct servent *serv;
  static char protocol[10], service[10];

  printf("Enter service to look up: ");
  scanf("%9s", service);
  
  printf("Enter protocol to look up: ");
  scanf("%9s", protocol);
  
  serv = getservbyname(service, protocol);
    
  if ( serv != (struct servent *) NULL ) {
    printf("Here is what I found about %s :\n", service);
    
    printf("Official name: %s\n", serv->s_name);
    printf("Aliases      : ");

    while ( *serv->s_aliases ) {
      printf("%s  ", *serv->s_aliases);
      ++serv->s_aliases;
    }
    printf("\nPort number   : %i\n", ntohs(serv->s_port));
    printf("Protocol Family: %i\n", serv->s_proto);
  }
  else
    printf("Service %s for protocol %s not found\n", service, protocol);
}
