#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 100000
#define sa struct sockaddr

struct sstone_addr
{
	char ip[20];
	int port;
	char url[100];
};



 void client(struct sockaddr_in addr,struct sstone_addr h[],int total,char url[],int conn)
{
	int sockfd;
	char buffer[MAX],buff1[MAX];
	
	int i;
	char ch;
	
	struct sockaddr_in servaddr;
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
		printf("unable to create the socket");
	bzero(&servaddr,sizeof(servaddr));
	servaddr=addr;
	if(connect(sockfd, (sa*) &servaddr,sizeof(servaddr))==-1)
	{
		printf("cannot be connected\n");
		return;
	}
	char ul[1024];

	
	strcpy(ul,url);
	int len=sizeof(buffer);
	send(sockfd,&ul,sizeof(ul),0);
	printf("in client function\n");
	
	len=sizeof(total);
	send(sockfd,&total,len,0);
	
	for(i=0;i<total;i++)
		send(sockfd,&h[i],sizeof(h[i]),0);
	
	
	char filename[120];
	recv(sockfd,&filename,sizeof(filename),0);
	send(conn,&filename,sizeof(filename),0);
	int j;
	i=0;
	char line[1024];
	 len=0;

		int bwrite;
		int bread;
		char lsend[MAX];
		i=0;
		int sz;
	recv(sockfd,&sz,sizeof(sz),0);
	send(conn,&sz,sizeof(sz),0);
	
	printf("Relaying the file\n");
	while(sz>0)
	{
		bzero(line,1024);
		
		i++;
	
		len=recv(sockfd,line,1024,0);
		sz=sz-len;
		
		
		if(len==0)
			break;
			
		
			
		bwrite=send(conn,line,len,0);
		
			
			
	}
	
	ccomp:printf("file sent......");
	printf("");
	

	
}
void randomize(struct sstone_addr h[],int total, char url[],int conn)
{
	int r,i,j;
	srand(time(NULL));
	r=rand()%total;
	printf("\n next ss: %s---%d \n",h[r].ip,h[r].port);
	printf("relaying the file....\n");
	struct sockaddr_in serv;
	serv.sin_family=AF_INET;
	inet_pton(AF_INET,h[r].ip,&serv.sin_addr);
	serv.sin_port=htons(h[r].port);

	struct sstone_addr h1[total-1];	
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
	client(serv,h1,total-1,url,conn);
	
	printf("Good bye.....");
}

//this is responsible for downloading the file

void download(int conn,char buff[])
{

	int j,i=0,k=0;        
        char mycommand[1000];
	char a[100],b[100];
	char ch;
	int ch1;
	FILE *fp;
	
        sprintf(mycommand,"wget %s",buff);
        
        system(mycommand);
	char rm[100];
	
	for(j=strlen(buff)-1;buff[j]!='/';j--)
	{			
	a[i++]=buff[j];

	}
		a[i]='\0';
	if(buff[j+1]='/')
		strcpy(b,"");
	i--;	
	
	for(;a[i]!='\0';i--)
	{
		b[k++]=a[i];
	}
	b[k]='\0';
	printf("\nfile name before comparing:%s\n",b);
	if( (strcmp(b,"")==0))
		strcpy(b,"index.html");

	printf("\nsending the file:%s\n",b);
	sprintf(rm,"rm %s",b);
	
	fp=fopen(b,"rb");
	if(fp==NULL)
	{
		printf("file cannot be opened");
		exit(0);
		//return;
	}
	printf("\nfile is opened\n");
	char line[1024];
	send(conn,b,strlen(b),0);	
	printf("sending the file");
	
	int len;


int bwrite;
int sz;
	i=0;
	fseek(fp,0L,SEEK_END);
	sz=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	send(conn,&sz,sizeof(sz),0);
int offset=0;
	while(sz>0)
	{

		bzero(line,1024);
		len=fread(line,1,1024,fp);
		sz=sz-len;
		
		offset=len;
		i++;
		if(len==-1)
		{
			printf("error in reading the data");
			exit(0);
		}

		if(len<1024)
		{
		
			bwrite=send(conn,line,len,0);
			break;
		}
		
			if(bwrite=send(conn,line,len,0)==-1)
			{
			printf("bwrite:%d",bwrite);
			perror("error occured");
				
			}
	
	len-=bwrite;
	


	}

	comp:printf("\nsending file completed");
	system(rm);
	fclose(fp);

	

}




