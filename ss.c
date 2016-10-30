#include<sys/types.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>
#include<netdb.h>
#include<pthread.h>

#define sa struct sockaddr

#include "awget.h"
void getmyip();
void * work(void *);
struct packet 
{
	uint16_t version;
	uint16_t length;
	char msg[140];
}  spack,rpack, nbyspack;
char ipaddress[100],hostname[1024];
struct sockaddr_in servaddr,cliaddr,local;
int sockfd,conn,clilen;
long t=0;
void main(int argc,char * argv[])
{
	
	int len;
	int sasize,c,i;
	char buff[MAX],msg[140],sbuff[MAX],rbuff[MAX];
	pthread_t thread;
	char bu[1000];
	bzero(&servaddr,sizeof(servaddr));
	
	//creating a socket
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	
	if(sockfd==-1)
		printf("\nproblem with socket creation");

	

	servaddr.sin_family=AF_INET;
	if(argc==1)
	{
		printf("usage:ss -p[port]");
		return;
	}
	if((c=getopt(argc,argv,"p"))!=-1)
		servaddr.sin_port=htons(atoi(argv[2]));
	getmyip();
	
	printf("\nserver is created at port:%d, ip:%s,\nhostname:%s",ntohs(servaddr.sin_port),ipaddress,hostname);

 inet_ntop(AF_INET,&servaddr.sin_addr,ipaddress,sizeof(ipaddress));	

	if(bind(sockfd,(sa*) &servaddr, sizeof(servaddr))==-1)
	{
		close(sockfd);

		printf("\nbinding failed");
		return;
	}
	
	

	while(1)
	{
		

		if(listen(sockfd,5)==-1)
			printf("\n listen failed");

		bzero(&cliaddr,sizeof(cliaddr));
		clilen=sizeof(cliaddr);
		if((conn=accept(sockfd,(sa*) &cliaddr, &clilen))==-1)	
			printf("\naccept failed");

		int rc;
 		rc = pthread_create(&thread, NULL, work, (void *)&conn);
      if (rc)
	{
         printf("ERROR; return code from pthread_create() is %d\n", rc);
	}

	}


	
	printf("good bye......\n");
	pthread_exit(NULL);
	
	
}
void* work(void * arg)
{
	int i;

	int conn=( *((int*)arg));
	char url[1024];
	recv(conn,&url,sizeof(url),0);
	int total,count;	
	printf("\nRequest:%s\n",url);
	recv(conn,&total,sizeof(total),0);
	
	if(total==0)
	{
		printf("\nchain list is empty.......\n");
	
		printf("\ndownloading the file.......\n");
		download(conn,url);
		printf("file sent.....\ngudbye\n");
		
	}
	else
	{

	
		struct sstone_addr h[total],temp;
		
		 for(i=0;i<total;i++)
		{
			
		
			bzero(&temp,sizeof(temp));
			recv(conn,&temp,sizeof(temp),0);
			
			strcpy(h[i].ip,temp.ip);
			h[i].port=ntohs(temp.port);
			
		}
		
		
	
		printf("chain list is.....");
		for(i=0;i<total;i++)
			printf("\n%s-------%d\n",h[i].ip,h[i].port);

		randomize(h,total,url,conn);
		printf("file sent...... good bye.......");

	}
	printf("file sent");
}
void getmyip()
{

	 int i;
    struct hostent *he;
    struct in_addr **addr_list;

       gethostname(hostname,100);
    if ((he = gethostbyname(hostname)) == NULL) {  
        herror("gethostbyname");
        return;
    }

    addr_list = (struct in_addr **)he->h_addr_list;
        strcpy(ipaddress, inet_ntoa(*addr_list[0]));

}










