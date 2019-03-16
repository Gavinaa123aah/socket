#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>       //pthread_t , pthread_attr_t and so on.
#include<stdio.h>
#include<netinet/in.h>      //structure sockaddr_in
#include<arpa/inet.h>       //Func : htonl; htons; ntohl; ntohs
#include<assert.h>          //Func :assert
#include<string.h>          //Func :memset
#include<unistd.h>          //Func :close,write,read
#define SOCK_PORT 9988
#define BUFFER_LENGTH 1024

int main()
{
    int sockfd;
    int tempfd;
    struct sockaddr_in s_addr_in;
    char data_send[BUFFER_LENGTH];
    char data_recv[BUFFER_LENGTH];
    memset(data_send,0,BUFFER_LENGTH);
    memset(data_recv,0,BUFFER_LENGTH);

    sockfd = socket(AF_INET,SOCK_STREAM,0);       //ipv4,TCP
    if(sockfd == -1)
    {
        fprintf(stderr,"socket error!\n");
        exit(1);
    }

    //before func connect, set the attr of structure sockaddr.
    memset(&s_addr_in,0,sizeof(s_addr_in));
    s_addr_in.sin_addr.s_addr = inet_addr("127.0.0.1");      //trans char * to in_addr_t
    s_addr_in.sin_family = AF_INET;
    s_addr_in.sin_port = htons(SOCK_PORT);

    tempfd = connect(sockfd,(struct sockaddr *)(&s_addr_in),sizeof(s_addr_in));
    if(tempfd == -1)
    {
        fprintf(stderr,"Connect error! \n");
        exit(1);
    }
    printf("#Exit (exit client command!)\n");
    printf("#Help (list Command)\n");     
    printf("#ListU (list user in a server\n");
    printf("#ListF (list files in a server)\n");
    printf("#TrfU ( transfert Upload file in a server)\n");
    printf("#TrfD ( transfert Download file in a server\n");
    while(1)
    {   
        
        printf("Please input something you wanna say(input \"Help\" to get help command):\n");
        gets(data_send);
        printf("---------------------------------\n");
        // tempfd = write(sockfd,data_send,BUFFER_LENGTH);
        // if(tempfd == -1)
        // {
        //     fprintf(stderr,"write error\n");
        //     exit(0);
        // }

        char *TrfU="TrfU";
        char *TrfD="TrfD";
        char *Exit="Exit";
        char *ListU="ListU";
        char *ListF="ListF";
        char *Help="Help";

        if(strcmp(data_send,Help)==0)
        {
           
            printf("#Exit (exit client command!)\n");
            printf("#Help (list Command)\n");     
            printf("#ListU (list user in a server\n");
            printf("#ListF (list files in a server)\n");
            printf("#TrfU ( transfert Upload file in a server)\n");
            printf("#TrfD ( transfert Download file in a server\n");
            continue;
        }
        else if(strcmp(data_send,Exit) == 0)  //quit,write the quit request and shutdown client
        {  
            printf("client exit\n");
            break;
        }else if (strstr(data_send,TrfU)!=NULL) {
            /* code */
            /*
            step 1:send "TrfU aa.txt" to server
            step 2:recv "prepare_recv" from server
            step 3:send file to server
            step 4:server receive&&store file in server
            step 5:server tell client "finsh"
            step 6:client over
            */
            char sdbuf[BUFFER_LENGTH];
            
            write(sockfd,data_send,BUFFER_LENGTH);
            // printf("Capture trfu: %s\n",data_send);
            read(sockfd,data_recv,BUFFER_LENGTH);

            //string 拼接 TrsfU aa.txt --> client_dir/aa.txt
            char *fs_name = malloc(strlen("client_dir/") + strlen(data_send) + 1 -5);
            strcpy(fs_name, "client_dir/");//拼接
            char *file_name_real = malloc(strlen(data_send)-5);
            strncpy(file_name_real, data_send+5, strlen(data_send)-5);//截取
            strcat(fs_name,file_name_real);//拼接
            //fs_name: client_dir/filename
            if (strstr(data_recv,"prepare_recv")!=NULL) {
                FILE *fs = fopen(fs_name, "r");
		        if(fs == NULL)
		        {
		            printf("ERROR: File %s not found.\n", fs_name);
                    bzero(file_name_real, strlen(file_name_real));
                    bzero(fs_name, strlen(fs_name));
                    continue;
		        }else//open file successfully
                {
                    // printf("file open sucess\n");
                    bzero(sdbuf, BUFFER_LENGTH);
		            int fs_block_sz;
                    while((fs_block_sz = fread(sdbuf, sizeof(char), BUFFER_LENGTH, fs))>0)
	                {
	                    if(send(sockfd, sdbuf, fs_block_sz, 0) < 0)
	                    {
	                        printf("ERROR: Failed to send file %s.\n", fs_name);
	                        break;
	                    }
	                    bzero(sdbuf, BUFFER_LENGTH);
	                 }
	                 printf("Ok File %s from Client was Sent!\n", fs_name);
                }
                
                /* code */
            }   
          
            continue;
        }else if (strstr(data_send,ListU)!=NULL)
        {  
            /* code */
            printf("Capture listU \n");
            continue;
        }else if (strstr(data_send,ListF)!=NULL)
        {
            /* code */
            
            printf("Files in server: \n");
            tempfd = write(sockfd,data_send,BUFFER_LENGTH);
            if(tempfd == -1)
            {
                fprintf(stderr,"write error\n");
                exit(0);
            }
            tempfd = read(sockfd,data_recv,BUFFER_LENGTH);
            assert(tempfd != -1);
            printf("%s\n",data_recv);
            memset(data_send,0,BUFFER_LENGTH);
            memset(data_recv,0,BUFFER_LENGTH);
            continue;
        }else if (strstr(data_send,TrfD)!=NULL) {
            printf("Capture trfD \n");
            continue;
        }else//write&&receive data from server
        {   char *mys = "mys";
            char *target = malloc(strlen(mys) + strlen(data_send) + 1);
            strcpy(target, mys);
            strcat(target,data_send);
            tempfd = write(sockfd,target,BUFFER_LENGTH);
            // tempfd = write(sockfd,data_send,BUFFER_LENGTH);
            if(tempfd == -1)
            {
                fprintf(stderr,"write error\n");
                exit(0);
            }
            tempfd = read(sockfd,data_recv,BUFFER_LENGTH);
            assert(tempfd != -1);
            printf("%s\n",data_recv);
            memset(data_send,0,BUFFER_LENGTH);
            memset(data_recv,0,BUFFER_LENGTH);
           
        }
        
    }

    int ret = shutdown(sockfd,SHUT_WR);       //or you can use func close()--<unistd.h> to close the fd
    assert(ret != -1);
    return 0;
}

