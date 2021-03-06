#include<stdlib.h>
#include<pthread.h>
#include<sys/socket.h>
#include<sys/types.h>       //pthread_t , pthread_attr_t and so on.
#include<stdio.h>
#include<netinet/in.h>      //structure sockaddr_in
#include<arpa/inet.h>       //Func : htonl; htons; ntohl; ntohs
#include<assert.h>          //Func :assert
#include<string.h>          //Func :memset
#include<unistd.h>          //Func :close,write,read
#include <dirent.h>   
#include <signal.h>
#include <ctype.h> 
#include <unistd.h> 
#include <errno.h>
#include <sys/wait.h>
#include <netdb.h>

#define SOCK_PORT 9988
#define BUFFER_LENGTH 1024
#define MAX_CONN_LIMIT 512     //MAX connection limit

static void Data_handle(void * sock_fd);   //Only can be seen in the file

int main()
{
    int sockfd_server;
    int sockfd;
    int fd_temp;
    struct sockaddr_in s_addr_in;
    struct sockaddr_in s_addr_client;
    int client_length;
   

    sockfd_server = socket(AF_INET,SOCK_STREAM,0);  //ipv4,TCP
    assert(sockfd_server != -1);

    //before bind(), set the attr of structure sockaddr.
    memset(&s_addr_in,0,sizeof(s_addr_in));
    s_addr_in.sin_family = AF_INET;
    s_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);  //trans addr from uint32_t host byte order to network byte order.
    s_addr_in.sin_port = htons(SOCK_PORT);          //trans port from uint16_t host byte order to network byte order.
    fd_temp = bind(sockfd_server,(struct scokaddr *)(&s_addr_in),sizeof(s_addr_in));
    if(fd_temp == -1)
    {
        fprintf(stderr,"bind error!\n");
        exit(1);
    }

    fd_temp = listen(sockfd_server,MAX_CONN_LIMIT);
    if(fd_temp == -1)
    {
        fprintf(stderr,"listen error!\n");
        exit(1);
    }

    while(1)
    {
        printf("waiting for new connection...\n");
        pthread_t thread_id;
        client_length = sizeof(s_addr_client);

        //Block here. Until server accpets a new connection.
        sockfd = accept(sockfd_server,(struct sockaddr_*)(&s_addr_client),(socklen_t *)(&client_length));
        if(sockfd == -1)
        {
            fprintf(stderr,"Accept error!\n");
            continue;                               //ignore current socket ,continue while loop.
        }
        printf("A new connection occurs!\n");
        if(pthread_create(&thread_id,NULL,(void *)(&Data_handle),(void *)(&sockfd)) == -1)
        {
            fprintf(stderr,"pthread_create error!\n");
            break;                                  //break while loop
        }
    }

    //Clear
    int ret = shutdown(sockfd_server,SHUT_WR); //shut down the all or part of a full-duplex connection.
    assert(ret != -1);

    printf("Server shuts down\n");
    return 0;
}

static void Data_handle(void * sock_fd)
{
    int fd = *((int *)sock_fd);
    int i_recvBytes;
    char data_recv[BUFFER_LENGTH];
    // char revbuf[BUFFER_LENGTH];//文件传输buffer
    const char * data_send = "Server has received your request!\n";

    while(1)
    {
        printf("waiting for request...\n");
        //Reset data.
        memset(data_recv,0,BUFFER_LENGTH);

        i_recvBytes = read(fd,data_recv,BUFFER_LENGTH);
        if(i_recvBytes == 0)
        {
            printf("Maybe the client has closed\n");
            break;
        }
        if(i_recvBytes == -1)
        {
            fprintf(stderr,"read error!\n");
            break;
        }
        if(strcmp(data_recv,"Exit")==0)
        {
            printf("exit command!\n");
            break;                           //Break the while loop.
        }else if (strstr(data_recv,"TrfU")!=NULL)
        {   char *prepare_recv = "prepare_recv";
            printf("enter TrfU\n");
            char *file_name_u = malloc(strlen(data_recv)-4);
            strncpy(file_name_u, data_recv+4, strlen(data_recv)-4);//截取client_dir/(filename)
            char *file_name_server = malloc(strlen(file_name_u)+11);
            strcpy(file_name_server, "server_dir/");//拼接 file_name_server="server_dir/"
            strcat(file_name_server,file_name_u);//拼接 file_name_server="server_dir/aa.txt"
            printf("%s\n",file_name_server);
            FILE *fr = fopen(file_name_server, "a");
            printf("upload filename:%s\n",file_name_u);    
            if(fr == NULL)
			    printf("File %s Cannot be opened file on server.\n", file_name_u);             
            else
            {
            write(fd,prepare_recv,strlen(prepare_recv));
            bzero(data_recv, BUFFER_LENGTH); 
			int fr_block_sz = 0;
			while((fr_block_sz = recv(fd, data_recv, BUFFER_LENGTH, 0)))//could it be sockfd?
			    {
			        if(fr_block_sz < 0)
			        {
			            printf("Error receiving file from client to server.\n");
			        }
			        int write_sz = fwrite(data_recv, sizeof(char), fr_block_sz, fr);
			        if(write_sz < fr_block_sz)
			        {
			            printf("File write failed on server.\n");
			        }
					else if(fr_block_sz)
					{
						break;
					}
			        bzero(data_recv, BUFFER_LENGTH); 
			    }
			printf("Ok received from client!\n");
			fclose(fr);
            }
            
           
            continue;
            /* code */
        }else if (strstr(data_recv,"TrfD")!=NULL)
        {   
            printf("download file\n");
            continue;
            /* code */
        }else if (strstr(data_recv,"ListF")!=NULL)
        {   
            printf("list files name\n");
            DIR *d;
            struct dirent *dir;
            d = opendir("server_dir");
            char file_names[1000];
            if (d)
            {
                while ((dir = readdir(d)) != NULL)
                {
                    strcat(file_names,dir->d_name);
                    strcat(file_names,"\n");
                }
                closedir(d);
                write(fd,file_names,strlen(file_names));
            }
            bzero(file_names, 1000); 
            continue;
            /* code */
        }else if (strstr(data_recv,"ListU")!=NULL)
        {   
            printf("List Users\n");
            continue;
            /* code */
        }else if (strstr(data_recv,"mys")!=NULL)//get usual string
        {
            /* code */
            char *target = malloc(strlen(data_recv)-3);
            strncpy(target, data_recv+3, strlen(data_recv)-3);
            printf("read from client : %s\n",target);
        }

        if(write(fd,data_send,strlen(data_send)) == -1)
        {
            printf("server write error!!\n");
            break;
        }
    }

    //Clear
    printf("terminating current client_connection...\n");
    close(fd);            //close a file descriptor.
    pthread_exit(NULL);   //terminate calling thread!
}