#include "includes/include.h"

int main() {

    system("clear");
    char command[256];
    int index = 0;     
    int history_count = 0;
    char ch;
    char *history[HISTORY_SIZE];

    do{
        display();
        label:
        while(1) {
            ch = getchar();
            if(ch == '\n') {
                command[index] = '\0';
                if(strcmp(command,"history") == 0) {
                    show_history(history,history_count);
                    break;
                }
                else {
                    add_history(history,&history_count,command);
                    execute_command(strdup(command));
                    break;
                }
            }
            if(ch == '\t') {
                command[index] = '\0';
                list_files(command);
                index = 0;
                goto label;
                break;
            }
            else {
                command[index++] = ch;
            }
        }
        index = 0;
    }while(1);
    return 0;
}