#include "local.h"

main(int argc, char *argv[])
{
  int orig_sock, len;
    
  static struct sockaddr_in serv_adr;

  struct hostent *host;

  if ( argc != 2 ) {
    fprintf(stderr, "usage: %s server\n", argv[0]);
    exit(1);
  }
  
  host = gethostbyname(argv[1]);

  if ( host == (struct hostent *) NULL ) {
    perror("gethostbyname ");
    exit(2);
  }

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family      = AF_INET;
  memcpy(&serv_adr.sin_addr, host->h_addr, host->h_length);
  serv_adr.sin_port        = htons(PORT);
  
  if ( (orig_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    perror("generate error");
    exit(3);
  }
  
  if ( connect(orig_sock, (struct sockaddr *) &serv_adr,
	       sizeof(serv_adr)) < 0 ) {
    perror("connect error");
    exit(4);
  }
  
  do {
    write(fileno(stdout), "> ", 3);
    
    if ( (len = read(fileno(stdin), buf, BUFSIZ)) > 0 ) {
      write(orig_sock, buf, len);
      
      if ( (len = read(orig_sock, buf, len)) > 0 )
	write(fileno(stdout), buf, len);
    }
  } while ( buf[0] != '.' );
  
  close(orig_sock);
  exit(0);
}
