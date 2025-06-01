#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int status = 1;

// Environment variable
char shell_path[1024];
char *args[MAX_ARGS];
char input[MAX_INPUT];
char *token;  

int main(int argc, char *argv[]) {
    
    // Set shell environment variable
    getcwd(shell_path, sizeof(shell_path));
    strcat(shell_path, "/OurShell");
    setenv("shell", shell_path, 1);

    myhelp();
    
    //Main loop
    while (status) {
        // Display prompt with current directory
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));

        printf("\033[1;95;45mOurShell:%s$\033[0m ", cwd); 
        
        // Read input
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            break;  // Exit on EOF (Ctrl+D)
        }
        
        // Remove newline character
        input[strcspn(input, "\n")] = '\0';
        
        // Skip empty input
        if (strlen(input) == 0) {
            continue;
        }

         // Tokenize input
         int i = 0;
         token = strtok(input, " ");
         while (token != NULL && i < MAX_ARGS - 1) {
             args[i++] = token;
             token = strtok(NULL, " ");
         }
         args[i] = NULL;
         
         // Handle internal commands
         if (strcmp(args[0], "mycd") == 0) {
             mycd(args[1]);
         } else if (strcmp(args[0], "myclr") == 0) {
             myclr();
         } else if (strcmp(args[0], "mydir") == 0) {
             mydir(args[1]);
         } else if (strcmp(args[0], "myenviron") == 0) {
             myenviron();
         } else if (strcmp(args[0], "myecho") == 0) {
             // Combine all arguments after myecho
             if (i > 1) {
                char comment[MAX_INPUT] = "";
                for (int j = 1; j < i; j++) {
                    strcat(comment, args[j]);
                    if (j < i - 1) strcat(comment, " ");
                }
                myecho(comment);
            } else {
                myecho("");
            }
        } else if (strcmp(args[0], "myhelp") == 0) {
            myhelp();
        } else if (strcmp(args[0], "mypause") == 0) {
            mypause();
        } else if (strcmp(args[0], "myquit") == 0) {
            myquit();
        } else if (strcmp(args[0], "myone") == 0) {
            myone();
        } else {
            // External command execution
            execute_command(args);
        }
    }   
    return 0;
}

void execute_command(char *args[]) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
    } else if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("Command execution failed!");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        wait(NULL);
    }
}
void mycd(char *path) {
    if (path == NULL) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd );
        printf("\n");
    } else {
        if (chdir(path) != 0) {
            perror("mycd error");
        }
    }
}
void myclr() {
    printf("\033[2J\033[H");  // ANSI escape codes to clear screen
}
void mydir(char *path) {
    DIR *dir;
    struct dirent *ent;
    char *dir_path = (path == NULL) ? "." : path;
    
    if ((dir = opendir(dir_path)) != NULL) {
        int n = 1;
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue; // Skip "." and ".."
            }
            printf("%d",n);
            printf(". %s\n", ent->d_name);
            n++;
        }
        closedir(dir);
    } else {
        perror("mydir error!");
    }
    printf("\n");
}
void myenviron() {
    extern char **environ;
    for (char **env = environ; *env; env++) {
        printf("%s\n", *env);
    }
    printf("\n");
}
void myecho(char *comment) {
    printf("%s\n", comment);
    printf("\n");
}
void myhelp() {
    printf("\n\e[4m\e[1mOurShell User Manual\e[m\e[0m\n");
    printf("\n");
    printf("INTERNAL COMMANDS:\n");
    printf("\n");
    printf("1. mycd <directory> - The command changes from the current working directory, to the one provided. NB: no argument shows the current directory! \n");
    printf("2. myclr - The command takes no arguments and clears the screen. \n");
    printf("3. mydir <directory> - The command lists the provided directory contents. NB: -> default directory is the current working directory. \n");
    printf("4. myenviron - The command lists all environment variables. \n");
    printf("5. myecho <comment> - The command displays the provided commend. \n");
    printf("6. myhelp - The command displays the user manual. \n");
    printf("7. mypause - The command pauses the shell operation until the 'Enter' key on the keyboard is pressed. \n");
    printf("8. myquit - The command terminates the shell. \n");
    printf("9. myone - A custom command that shows file count, the files and their modification time in the current directory. \n");
    printf("\n\e[3mExternal commands can also be executed directly!\e[m\n\n");
}
void mypause() {
    printf("Shell operation has been paused! Press \e[3m\e[1mEnter\e[m\e[m to continue......\n");
    while (getchar() != '\n');
}
void myquit() {
    printf("Terminating OurShell......\n");
    status = 0;
}
void mydir2(char *path, int verbose) {
    DIR *dir;
    struct dirent *ent;
    char *dir_path = (path == NULL) ? "." : path;

    // First, count entries
    int count = 0;
    if ((dir = opendir(dir_path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                count++;
            }
        }
        closedir(dir);
    } else {
        perror("mydir2 error!");
        return;
    }

    // Allocate array to store names
    char **names = malloc(sizeof(char*) * count);
    int i = 0;

    // Reopen and collect names
    if ((dir = opendir(dir_path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                names[i] = strdup(ent->d_name);
                i++;
            }
        }
        closedir(dir);
    }

    // Sort names alphabetically
    for (int j = 0; j < count - 1; j++) {
        for (int k = j + 1; k < count; k++) {
            if (strcmp(names[j], names[k]) > 0) {
                char *temp = names[j];
                names[j] = names[k];
                names[k] = temp;
            }
        }
    }

    // Print sorted entries
    for (int j = 0; j < count; j++) {
        printf("%d. ", j + 1);

        if (verbose) {
            char fullpath[1024];
            snprintf(fullpath, sizeof(fullpath), "%s/%s", dir_path, names[j]);

            struct stat st;
            if (stat(fullpath, &st) == 0) {
                char timebuf[64];
                struct tm *tm = localtime(&st.st_mtime);
                strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M", tm);
                printf("%s ", timebuf);
            } else {
                perror("stat error");
            }
        }

        printf("%s\n", names[j]);
        free(names[j]); // Free each name
    }

    free(names); // Free name array
    printf("\n");
}

void myone(){
    DIR *dir;
    struct dirent *ent;
    int count = 0;
    
    if ((dir = opendir(".")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                count++;
            }
        }
        closedir(dir);
        printf("Number of unhidden files in the current directory is: %d",count);
        printf("\n\nThe files are:\n");
        mydir2(".", 1);    
    } else {
        perror("Unable to open directory! \n");
    }
}

