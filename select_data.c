#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

int login_flag = 0;
char username[10];
char password[10];
int select();
static int callback_select(void *data, int argc, char **argv, char **azColName){
   // int i;
   // fprintf(stderr, "%s: ", (const char*)data);
   // for(i=0; i<argc; i++)
   // {
   //    if(strstr(azColName[i],"username")!=NULL){
   //       printf("username is :%s", argv[i]);

   //    }
      
   //    // printf("\n");
   //    // printf("%d\n",i);
   //    // printf("%s\n", azColName[i]);
   //    // printf("%s\n", argv[i]);
   // }
   if(strstr(argv[0],username)!=NULL)
   {
      if (strstr(argv[1],password)!=NULL) {
         /* code */
        
         login_flag = 1;
      }      
   }
   return 0;
}

int select()
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called";

   /* Open database */
   rc = sqlite3_open("test.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create SQL statement */
   sql = "SELECT * from users";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback_select, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
   return 0;
}

int main()
{  
   // char username[10];
   // char password[10];
   while(1){
      printf("please input username:");
      gets(username);
      printf("\nplease input password:");
      gets(password);
      select();
      if (login_flag==1) {
         printf("login sucess\n");
         break;
      }else
      {
         printf("username or password worong!\n");
         continue;
      }
   }

   printf("ok!!!\n");
   // printf("\nusername is %s",username);
   // printf("\npassword is %s\n",password);
   
   
   // select();
   // if(login_flag==1){  printf("login sucess!\n");}
   // else
   // {
   //     printf("login error!\n");
   // }
   
  
   return 0;
}
