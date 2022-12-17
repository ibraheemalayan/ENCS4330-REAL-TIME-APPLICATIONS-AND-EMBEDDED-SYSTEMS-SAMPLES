/*
 * Detaching from a thread informs the Pthreads library that no other
 * thread will use the pthread_join mechanism to synchronize with the
 * thread's exiting. Because the library doesn't preserve the exit
 * status of a detached thread, it can operate more efficiently and
 * make the library resources that were associated with a thread
 * available for reuse more quickly. If no other thread cares when a
 * particular thread in your program exits, consider detaching that
 * thread.
 */

pthread_attr_t detached_attr;   

.   

.   

.   

pthread_attr_setdetachedstate(&detached_attr, PTHREAD_CREATE_DETACHED);   

.   

.   

.   

pthread_create(&thread, &detached_attr, ...);   

.   

.   

.  
 

/*
 * You can set multiple individual attributes within a single
 * attribute object. In the next example, Example 4-5, we'll use calls
 * to the pthread_attr_setstacksize function and the
 * pthread_attr_setdetachedstate function to set a thread's stack size
 * and detached state in the same object.
 */

pthread_attr_t custom_attr;   

.   

pthread_attr_init(&custom_attr);   

.   

pthread_attr_setstacksize(&custom_attr, MIN_REQ_SSIZE);   

pthread_attr_setdetachedstate(&custom_attr, PTHREAD_CREATE_DETACHED);   

.   

.   

pthread_create(&thread, &custom_attr, ...);   

.   

.   

.   
 
  
 
