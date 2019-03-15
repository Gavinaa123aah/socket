#include <dirent.h>
#include <stdio.h>
#include <string.h>
 
int main(void)
{
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
            // printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
    printf("file name:\n%s",file_names);
    
    return(0);
}