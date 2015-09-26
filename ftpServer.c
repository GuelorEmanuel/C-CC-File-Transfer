#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "ftpDefs.h"

void ls(char **output) {
  DIR* directory = opendir("./");
  if (directory == NULL) {
    printErrorMsg("opendir() failed");
  }

  struct dirent *entry = NULL;
  while ((entry = readdir(directory)) != NULL) {
    strcat(*output, entry->d_name);
    strcat(*output, "\n");
  }

  closedir(directory);
}

void get(char *filename, char **output) {
  FILE* file = fopen(concat("./", filename), "rb");
  if (file == NULL) {
    printErrorMsg("open() failed");
  }

  while (1) {
    int bytesRead = fread(*output, 1, MAX_BUFF_LEN, file);
    if (bytesRead < 0) {
      printErrorMsg("fread() failed");
    }
    if (bytesRead < MAX_BUFF_LEN) {
      if (ferror(file)) {
        printErrorMsg("something bad happened.");
      }
      if (feof(file)) {
        break;
      }
    } else {
      // TODO!!
    }
    fclose(file);
  }

}

int makeDirectory(char *name, char **output) {
  int result = mkdir(name, 0777);
  if (result == 0) {
    sprintf(*output, "Directory `%s` created.\n", name);
  } else {
    sprintf(*output, "Failed to created directory `%s`.\n", name);
  }
  return result;
}

void processRequest(char *request, char **reply) {
  bzero(*reply, MAX_BUFF_LEN);

  char* trimBuff = trimStringAfter(request);
  char* mergeStringForMakeDirectory = concat("mkdir ", trimBuff);
  char* mergeStringForChngDirectory = concat("cd ", trimBuff);
  char* mergeStringForGet           = concat("get ", trimBuff);

  if ((strcmp(request, "ls")) == 0) {
    ls(reply);
  }
  else if ((strcmp(request, mergeStringForMakeDirectory)) == 0) {
    makeDirectory(trimBuff, reply);
  }
  else if ((strcmp(request, mergeStringForGet)) == 0) {
    get(trimBuff, reply);
  }
  else {
    sprintf(*reply, "%s: command not found\n", request);
  }

  free(mergeStringForMakeDirectory);
  free(mergeStringForChngDirectory);
}

int main(int argc, char *argv[]) {
  // Do not continue unless all arguments have been provided
  if (argc < 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  int port_number = atoi(argv[1]), listen_socket = -1;
  setupListenSocket(port_number, &listen_socket);
  printf("Accepting connections on port %d.\n", port_number);
  fflush(stdout);

  int accept_socket = -1;
  acceptIncomingConnection(&listen_socket, &accept_socket);

  while (1) {
    char buffer[MAX_BUFF_LEN];
    receiveMessage(buffer, accept_socket);

    char *reply = malloc(MAX_BUFF_LEN);
    processRequest(buffer, &reply);
    sendMessage(reply, accept_socket);
    free(reply);
  }

  close(accept_socket);
  close(listen_socket);

  return 0;
}

