#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */

int main(int argc, char **argv)
{
  int pipefd[2];
  int pid;
  char* grep_arg = argv[1];
  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", grep_arg, NULL};

  // make a pipe (fds go in pipefd[0] and pipefd[1])

  pipe(pipefd);
  if (pipe(pipefd) == -1){
    fprintf(stderr, "pipe failed!");
    return 1;
  }

  pid = fork();

  if (pid == 0)
    {
      // child gets here and handles "grep Villanova"

      // replace standard input with input part of pipe
      int pid1;
      int pipefd1[2];

      if (pipe(pipefd1) == -1){
        fprintf(stderr, "pipe failed!");
        return 1;

      }

      char* sort_args[] = {"sort", NULL};

      close(pipefd[1]);


      // dup2(pipefd[0], 0);

      // // close unused hald of pipe

      // close(pipefd[1]);

      // // execute grep

      // execvp("grep", grep_args);

      pid1 = fork();
      if (pid1 == 0){
           dup2(pipefd1[0], 0);
          // close unused unput half of pipe
          close(pipefd1[1]);
          execvp("sort", sort_args);

      }else{
          dup2(pipefd[0], 0);
          // close unused unput half of pipe
          close(pipefd[1]);
          dup2(pipefd1[1], 1);
          // close unused unput half of pipe
          close(pipefd1[0]);
          execvp("grep", grep_args);

      }



    }
  else
    {
      // parent gets here and handles "cat scores"

      // replace standard output with output part of pipe

      dup2(pipefd[1], 1);

      // close unused unput half of pipe

      close(pipefd[0]);

      // execute cat

      execvp("cat", cat_args);
    }
}
