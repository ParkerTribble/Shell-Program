#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

char cmd[1024];
char* cmdptr = cmd;
char* arglist[10];
char* prompt = "Enter Command: ";
int cpid=-1,status;

void ctrlCtrap(int sig) {
  printf("Rcvd Ctrl-C\n");
  if (cpid>0) kill(cpid,2);
}

void dir();

void copy(int argc);

void runcmd(char *cmd, int argc) {
  int i=0;
  while (*cmd != '\n' && *cmd!='\0') {
    arglist[i++] = cmd;
    while (*cmd!=' ' && *cmd!='\n'){
      cmd++;
    }
    *cmd = '\0';
    cmd++;
  }
  arglist[i] = NULL;
  if(strcmp(arglist[0], "dir")==0) dir();
 if(strcmp(arglist[0], "copy")==0) copy(argc);
  execvp(arglist[0], arglist);
  printf("Error, command not found.\n");
  exit(0);
}

int main(int argc) {
  int c,n,i;

  signal(2,ctrlCtrap);
  while (1) {
    printf("%s",prompt);
    fgets(cmd,sizeof(cmd),stdin);
    if (strcmp(cmd,"exit\n")==0) break;
    cpid = fork();
    if (cpid==0) {
      runcmd(cmd,argc);
      exit(1);
    }
  waitpid(cpid,&status,0);
  }
}

void dir(){
  struct dirent *de;
  DIR *dir;
  char direct[256];

  printf("Enter the directory path ('.' will show current directory): ");
  scanf("%s",direct);

  if(direct!=NULL)
    {
      dir = opendir(direct);
    }

  if((dir = opendir(".")) == NULL)
    {
 printf("Cannot open Directory");
    }

  while((de = readdir(dir))!=NULL)
    {
      printf("%s\n", de->d_name);
    }
  closedir(dir);;
}

void copy(int argc) {
  int fdin, fdout = 0;
  char line[1024];
  int i, n;
  char *argv[3];
  char *files[2];

  printf("Enter file to copy: ");
  fgets(files[1],sizeof(files[1]),stdin);
 printf("Enter location for copied file \n");
  fgets(files[2],sizeof(files[2]), stdin);

   fdin = open(files[1], O_RDONLY);
  if (fdin < 0)
    {
      printf("Error, could not open%s\n", argv[1]);
    }

  if (argc >= 2)
    {
      fdout = open(files[2], O_WRONLY | O_CREAT, 0666);
    }
  while ((n = read(fdin, line, 1024)) > 0)
    {
      i = write(fdout, line, n);
    }
  close(fdin);
  close(fdout);
}
