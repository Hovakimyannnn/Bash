#include "../includes/include.h"

int alias_count = 0;
Alias aliases[ALIAS_COUNT];

void set_text_color(const char *color) {
    printf("%s", color);
}

void set_text_default() {
    printf("\033[0m");
}
void show_history(char **history, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d %s\n", i + 1, history[i]);
    }
}
void add_history(char **history, int *size, char *command) {
    if (*size < HISTORY_SIZE) {
        history[*size] = strdup(command);
        (*size)++;
    } else {
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++) {
            history[i - 1] = history[i];
        }
        history[HISTORY_SIZE - 1] = strdup(command);
    }
}

void get_username_hostname() {
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);

    if (pw) {
        printf("%s@", pw->pw_name);
    } else {
        perror("getpwuid error");
    }
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        printf("%s", hostname);
    } else {
        perror("gethostname error");
    }
}
void add_alias(char *name, char *value) {
    if (alias_count < ALIAS_COUNT) {
        strncpy(aliases[alias_count].alias_name, name, ALIAS_NAME - 1);
        strncpy(aliases[alias_count].alias_value, value, ALIAS_VALUE - 1);
        alias_count++;
    } else {
        printf("Too many aliases\n");
    }
}
char* find_alias(char *name) {
    for (int i = 0; i < alias_count; i++) {
        if (strncmp(aliases[i].alias_name, name,ALIAS_NAME - 1) == 0) {
            return aliases[i].alias_value;
        }
    }
    return NULL;
}
void execute_command(char *command){
    if (!command) exit(1);

    char *alias_value = find_alias(command);
    if (alias_value) {
        command = strdup(alias_value);
    }

    if (strcmp(command, "exit") == 0) exit(0);
    if(strcmp(command,"pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("getcwd error");
        }
        return;
    }
    if (strncmp(command, "cd ", 3) == 0) {
        char *path = command + 3;
        if (chdir(path) != 0) {
            perror("chdir error");
        }
        return;
    }

    pid_t id = fork();
    if (id < 0) {
        perror("fork error");
        exit(1);
    }
    if (id == 0) {
        char *args[256];
        int i = 0;
        char *token = strtok(command, " ");
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; 

        
        if (strcmp(args[0], "alias") == 0) {
            if (i >= 3) {
                add_alias(args[1], args[2]);
            } else {
                fprintf(stderr, "Invalid alias format. Usage: alias name value\n");
            }
            return;
        }
        if (execvp(args[0], args) == -1) {
            perror("execvp error");
            exit(1);
        }

    } else {
        wait(NULL);
    }
}
void display() {
    const char *GREEN = "\033[92m";
        const char *BLUE = "\033[94m";
        set_text_color(GREEN);
        get_username_hostname();
        set_text_default();
        printf(":");
        set_text_color(BLUE);
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s", cwd);
        } else {
            perror("getcwd error");
        }
        set_text_default();
        printf("$: ");
        fflush(stdin);
}
void list_files(const char *prefix) {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strncmp(dir->d_name, prefix, strlen(prefix)) == 0) {
                printf("%s\n", dir->d_name);
            }
        }
        closedir(d);
    }
}
/*void set_non_canonical_mode(struct termios *old_tio) {
    struct termios new_tio;
    tcgetattr(STDIN_FILENO, old_tio);
    new_tio = *old_tio;
    new_tio.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}
void reset_terminal_mode(struct termios *old_tio) {
    tcsetattr(STDIN_FILENO, TCSANOW, old_tio);
}*/
