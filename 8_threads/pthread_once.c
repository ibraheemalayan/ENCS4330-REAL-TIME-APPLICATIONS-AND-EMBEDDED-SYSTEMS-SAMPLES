pthread_once_t      srv_comm_inited_once = PTHREAD_ONCE_INIT;   

void server_comm_get_request(int *conn, char *req_buf)   
{
  int i, nr, not_done = 1;   
  fd_set read_selects;   
  
  pthread_once(&srv_comm_inited_once, server_comm_init);   
  
  while (not_done) {
    
    .   
 
    .   
 
    .   
}   
 
  
 
