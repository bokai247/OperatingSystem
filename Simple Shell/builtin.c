/* COMP 530: Tar Heel SHell
 *
 * This file implements a table of builtin commands.
 */
#include "thsh.h"
#include <stdlib.h>

struct builtin {
  const char * cmd;
  int (*func)(char *args[MAX_ARGS], int stdin, int stdout);
};


static char old_path[MAX_INPUT];
static char cur_path[MAX_INPUT];
static char temp_path[MAX_INPUT];

/* This function needs to be called once at start-up to initialize
 * the current path.  This should populate cur_path.
 *
 * Returns zero on success, -errno on failure.
 */
int init_cwd(void) {

  // Lab 1: Your code here
  if (getcwd(cur_path, sizeof(cur_path)) != NULL) {
      return 0;
   } else {
      return -errno;
   }
}

/* Handle a cd command.  */
int handle_cd(char *args[MAX_INPUT], int stdin, int stdout) {

  // Note that you need to handle special arguments, including:
  // "-" switch to the last directory
  // "." switch to the current directory.  This should change the
  //     behavior of a subsequent "cd -"
  // ".." go up one directory
  //
  // Hint: chdir can handle "." and "..", but saving
  //       these results may not be the desired outcome...

  // XXX: Be sure to write test cases for ., .., and weirdness
  // XXX: Test for errors in the output if a cd fails

  // Lab 1: Your code here
  char *path = args[1];

  if(path != NULL && strcmp(path, "-") == 0){    // cd -
    if(old_path == NULL){
      strcpy(old_path, cur_path);
    }
    strcpy(temp_path, old_path);
    getcwd(old_path, sizeof(old_path));
    chdir(temp_path);
  }

  if(path == NULL){                             // cd
    getcwd(old_path, sizeof(old_path));                     
    char *s = getenv("HOME");
    chdir(s);
  }

  if(path != NULL && strcmp(path, "-") != 0){   // general case
    getcwd(old_path, sizeof(old_path));
    chdir(path);                                
  }
  
  return 42;
}

/* Handle an exit command. */
int handle_exit(char *args[MAX_ARGS], int stdin, int stdout) {
  exit(0);
  return 0; // Does not actually return
}

/* Handle an goheels command. */
int handle_goheels(char *args[MAX_ARGS], int stdin, int stdout) {
    printf("\n");
    printf("                     ::::::::                              :: ::::::              \n");
    printf("               :::::         ::::::::::::::::::::::::::::::          ::::         \n");
    printf("               :::                                                    ::          \n");
    printf("                :::                                                  ::           \n");
    printf("             ::::                                                     ::::        \n");
    printf("           :::                                                            :::     \n");
    printf("         ::                           :::::::::::::::                        :::  \n");
    printf("       :::                               :::        ::                      :::   \n");
    printf("      :::                                   ::      ::          ::        :::     \n");
    printf("     :::            :::                       ::    ::          ::::::  :::       \n");
    printf("     ::           :: ::                         ::: ::          ::    ::          \n");
    printf("     ::           :   :           ::              ::::          ::                \n");
    printf("     ::           :   :           ::::              :           ::                \n");
    printf("      ::          ::: :           ::  ::                        ::   ::::         \n");
    printf("      :::           :::           ::    ::                      :::::   :::       \n");
    printf("       :::                        ::      ::                               ::     \n");
    printf("         :::                      :::::::::::::                             :::   \n");
    printf("           :::                                                             :::    \n");
    printf("             :::                                                       ::::       \n");
    printf("                :::                                                :::            \n");
    printf("                 ::                                                  ::           \n");
    printf("                ::            ::::::::::::::::::::::::::::           :::         \n");
    printf("                  :::::::   ::                           :::   ::::::             \n");
    printf("                          ::                               :::                    \n");
    printf("                                                                                \n");
    printf("\n");
    printf("      ___           ___           ___           ___           ___           ___       ___\n");
    printf("     /\\  \\         /\\  \\         /\\__\\         /\\  \\         /\\  \\         /\\__\\     /\\  \\    \n");
    printf("    /::\\  \\       /::\\  \\       /:/  /        /::\\  \\       /::\\  \\       /:/  /    /::\\  \\   \n");
    printf("   /:/\\:\\  \\     /:/\\:\\  \\     /:/__/        /:/\\:\\  \\     /:/\\:\\  \\     /:/  /    /:/\\ \\  \\  \n");
    printf("  /:/  \\:\\  \\   /:/  \\:\\  \\   /::\\  \\ ___   /::\\~\\:\\  \\   /::\\~\\:\\  \\   /:/  /    _\\:\\~\\ \\  \\ \n");
    printf(" /:/__/_\\:\\__\\ /:/__/ \\:\\__\\ /:/\\:\\  /\\__\\ /:/\\:\\ \\:\\__\\ /:/\\:\\ \\:\\__\\ /:/__/    /\\ \\:\\ \\ \\__\\\n");
    printf(" \\:\\  /\\ \\/__/ \\:\\  \\ /:/  / \\/__\\:\\/:/  / \\:\\~\\:\\ \\/__/ \\:\\~\\:\\ \\/__/ \\:\\  \\    \\:\\ \\:\\ \\/__/\n");
    printf("  \\:\\ \\:\\__\\    \\:\\  /:/  /       \\::/  /   \\:\\ \\:\\__\\    \\:\\ \\:\\__\\    \\:\\  \\    \\:\\ \\:\\__\\  \n");
    printf("   \\:\\/:/  /     \\:\\/:/  /        /:/  /     \\:\\ \\/__/     \\:\\ \\/__/     \\:\\  \\    \\:\\/:/  /  \n");
    printf("    \\::/  /       \\::/  /        /:/  /       \\:\\__\\        \\:\\__\\        \\:\\__\\    \\::/  /   \n");
    printf("     \\/__/         \\/__/         \\/__/         \\/__/         \\/__/         \\/__/     \\/__/    \n");
  return 1;
}

static struct builtin builtins[] = {{"cd", handle_cd},
				    {"exit", handle_exit},
                    {"goheels",handle_goheels},
				    {'\0', NULL}};

/* This function checks if the command (args[0]) is a built-in.
 * If so, call the appropriate handler, and return 1.
 * If not, return 0.
 *
 * stdin and stdout are the file handles for standard in and standard out,
 * respectively. These may or may not be used by individual builtin commands.
 *
 * Places the return value of the command in *retval.
 *
 * stdin and stdout should not be closed by this command.
 *
 * In the case of "exit", this function will not return.
 */
int handle_builtin(char * args[MAX_ARGS], int stdin, int stdout, int * retval) {
  int rv = 0;
  // Lab 0: Your Code Here
  int i = 0;
  // loop through all builtin command list
  while (builtins[i].cmd) {
    // if the input args matches one of the command
    if (strcmp(*args, builtins[i].cmd) == 0) {
      // save ans and return 1
      int ans = builtins[i].func(args, stdin, stdout);
      * retval = ans;
      return 1;
    }
    i++;
  }

  (void) builtins;
  return rv;
}

/* This function initially prints a default prompt of:
 * thsh>
 *
 * In Lab 1, Exercise 3, you will add the current working
 * directory to the prompt.  As in, if you are in "/home/foo"
 * the prompt should be:
 * [/home/foo] thsh>
 *
 * Returns the number of bytes written
 */
int print_prompt(void) {
  int ret = 0;
  // Print the prompt
  // file descriptor 1 -> writing to stdout
  // print the whole prompt string (write number of
  // bytes/chars equal to the length of prompt)
  //
  char prompt[MAX_INPUT];

  // Lab 1: Your code here
  init_cwd();
  snprintf(prompt, sizeof(prompt), "%s%s%s%s", "[", cur_path, "]", " thsh> ");

  ret = write(1, prompt, strlen(prompt));
  return ret;
}
