#include "local.h"

main(void)
{
  int orig_sock, new_sock, clnt_len;
  

  struct sockaddr_in clnt_adr, serv_adr;
  int len, i;
  
  if ( (orig_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    perror("generate error");
    exit(1);
  }
  
  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family      = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port        = htons(PORT);

  if ( bind(orig_sock, (struct sockaddr *) &serv_adr,
	    sizeof(serv_adr)) < 0 ) {
    perror("bind error");
    close(orig_sock);
    exit(2);
  }
  
  if ( listen(orig_sock, 5) < 0 ) {
    perror("listen error");
    close(orig_sock);
    exit(3);
  }
  
  do {
    clnt_len = sizeof(clnt_adr);
    
    if ( (new_sock = accept(orig_sock, (struct sockaddr *) &clnt_adr,
			    &clnt_len)) < 0 ) {
      perror("accept error");
      close(orig_sock);
      exit(4);
    }
    
    if ( fork() == 0 ) {
      while ( (len = read(new_sock, buf, BUFSIZ)) > 0 ) {
	for ( i = 0; i < len; i++ )
	  buf[i] = toupper(buf[i]);
	
	printf("buf = %s", buf);
	write(new_sock, buf, len);
	/*sleep(10);*/
	
	if ( buf[0] == '.' ) 
	  break;
      }
      
      close(new_sock);
      exit(0);
    }
    else
      close(new_sock);
  } while (1);
}
