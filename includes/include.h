#ifndef TERMINAL_
#define TERMINAL_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <pwd.h>
#include <sys/types.h>

#define HISTORY_SIZE 20
#define ALIAS_COUNT 100
#define ALIAS_NAME 50
#define ALIAS_VALUE 200

typedef struct {
    char alias_name[ALIAS_NAME];
    char alias_value[ALIAS_VALUE];
} Alias;

void set_text_color(const char *color);
void set_text_default();
void get_username_hostname();
void show_history(char **history, int size);
void add_history(char **history, int *size, char *command);
void execute_command(char *command);
void reset_terminal_mode(struct termios *old_tio);
void set_non_canonical_mode(struct termios *old_tio);
void list_files(const char *prefix);
void display();
void add_alias(char *, char *);
void free_history(char **history, int size);

#endif // TERMINAL_