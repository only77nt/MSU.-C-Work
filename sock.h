#define BUFFER_SIZE 255 /*255 символов - стандартный размер буффера + 256 символ - '\0'*/
#define NAME_SIZE 30 /*30 символов вполне достаточно для никнейма*/

struct sockaddr_in server_address;
struct sockaddr_in client_address;
struct sockaddr_in address;

int str(char *, char *);
