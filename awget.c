#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netdb.h>
#include<fcntl.h>
#include "awget.h"

void randomize1(struct sstone_addr h[],int total, char url[]);
void client1(struct sockaddr_in addr,struct sstone_addr h[],int total,char url[]);

void main(int argc,char* argv[])
{
	FILE *fp;
	char line[1024];
	int count=0;
	int i,j;
	int c,r;
	int totalrec=1;
	char port[20],ip[20];
	char url[1024];
	char filename[1024];
	printf("argc=%d",argc);	
	if(argc==1)
	{
		printf("usage: awget <url> [-p chainfilename]");
		return;
	}
	else 
	if(argc==3)
	{
		if((c=getopt(argc,argv,"c"))!=-1)
		strcpy(filename,argv[2]);


	}
	else
		strcpy(filename,"chaingang.txt");

	strcpy(url,argv[1]);
	
	
	fp=fopen(filename,"r");
	if(fp==NULL)
	{
		printf("file cannot be opened\n");
		return;
	}
	if(fgets(line,sizeof(line),fp)!=NULL)
	{
			totalrec=atoi(line);
	
				count++;
				printf("\ntotal number of records:%d\n",totalrec);
	}



	printf("Request:%s",url);

	struct sstone_addr h[totalrec];

	while(fgets(line,sizeof(line),fp)!=NULL)
	{

			
			
						
		
		       		for(i=0;line[i]!=',';i++)
                       		         ip[i]=line[i];
                       		 ip[i]='\0';
				 j=0;
                       		 i++;
				printf("iteration:%d",count);
                        	for(;line[i]!='\0';i++)
                        	        port[j++]=line[i];
                        	port[j]='\0';
				strcpy(h[count-1].ip,ip);
                        	h[count-1].port=atoi(port);
				strcpy(h[count-1].url,url);
				printf("\n %s---->%d \n",h[count-1].ip,h[count-1].port);	
				count++;
			
	}
	fclose(fp);
	printf("total:%d",totalrec);

	randomize1(h,totalrec,url);
	
	
		
	
	

}
void randomize1(struct sstone_addr h[],int total, char url[])
{
	int r,i,j;
	srand(time(NULL));
	r=rand()%total;
	printf("\nnext ss: %s---%d \n",h[r].ip,h[r].port);
	struct sockaddr_in serv;
	serv.sin_family=AF_INET;
	inet_pton(AF_INET,h[r].ip,&serv.sin_addr);
	serv.sin_port=htons(h[r].port);
	struct sstone_addr h1[total-1];	
	//printf("remaining stepping stones.....\n");
	i=0,j=0;
	while(i<total-1)
	{
		
		if(j==r)
		{	j++;
			continue;
		}
		strcpy(h1[i].ip,h[j].ip);
		h1[i].port=htons(h[j].port);
		
		i++;
		j++;
			
	}
	
	client1(serv,h1,total-1,url);
}




void client1(struct sockaddr_in addr,struct sstone_addr h[],int total,char url[])
{
	int sockfd;
	char buffer[1024],buff1[MAX];
	
	int i,j;
	struct sockaddr_in servaddr;
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
		printf("unable to create the socket\n");
	bzero(&servaddr,sizeof(servaddr));
	servaddr=addr;
	if(connect(sockfd, (sa*) &servaddr,sizeof(servaddr))==-1)
	{
		printf("cannot be connected\n");
		return;
	}
	strcpy(buffer,url);
	int len=sizeof(buffer);
	printf("url sent:%s",url);
	send(sockfd,&buffer,len,0);
	len=sizeof(total);
	send(sockfd,&total,len,0);
	
	for(i=0;i<total;i++)
		send(sockfd,&h[i],sizeof(h[i]),0);
	char filename[120];
	char file[120];
	recv(sockfd,&filename,sizeof(filename),0);
	printf("\nfile name:%s\n",filename);
	sprintf(file,"downlod\\%s",filename);
	char line[1024];
	FILE *fp;
	
	fp=fopen(file,"wb");
	if(fp==NULL)
	{
		printf("file cannot be opened\n");
		exit(0);
	}

		
	len=0;		
	
	int bwrite;
	int bread;
	char ch;
	i=0;
	int sz;
	recv(sockfd,&sz,sizeof(sz),0);
	

	while(sz>0)
	{
		
		bzero(line,1024);
		
		len=recv(sockfd,line,1024,0);		
			if(len==0)
			break;
		
        
        fwrite(line,len,1,fp);
		i++;
			
	}

	

comp:	printf("\nfile received successfully");
	printf("\nGood Bye\n");
	fclose(fp);

	
	
	
}

	


	



