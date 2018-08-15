#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// char* = string (array of chars)
// (char*)* = array of strings
int main(int arg_c, char** arg_value)
{
	pid_t pid;

	// File descriptors are basically pipelines from one process to another
	int bucket1[2];
	int bucket2[2];
	// Create the <ps | grep> pipe
	pipe(bucket1);
	// Create the <grep | wc> pipe
	pipe(bucket2);

	// Attempt a fork
	pid = fork();
	// At this point, the parent and child are both
	// running the same code, but pid is only 0 in the child
	// so the parent skips this part and jumps to the wait
	if (pid == 0) {
		// The child starts executing this part
		// Alias/redirect stdout in the child into the first pipe
		dup2(bucket1[STDOUT_FILENO], STDOUT_FILENO);
		// call exec, replace the process image with "ps"
		execlp("ps", "ps", "-A", NULL);
		// "ps" should exit successfully here.
		// if the process image was replaced, the exit(1) will not be here
		// if the process image was not replaced, exit unsuccessfully
		_exit(1);
	}

	// Wait for forked subprocess to cleanly exit
	wait(NULL);

	// Attempt a fork
	pid = fork();
	// At this point, the parent and child are both
	// running the same code, but pid is only 0 in the child
	// so the parent skips this part and jumps to the wait
	if (pid == 0) {
		// The child starts executing this part
		// Alias/redirect stdin in the child to be the data from the first bucket
		// (that had stdout piped into it)
		dup2(bucket1[STDIN_FILENO], STDIN_FILENO);

		// No longer need to write into bucket1, so can close the fd.
		close(bucket1[STDOUT_FILENO]);

		// Redirect stdout into the next bucket, destined for wc
		dup2(bucket2[STDOUT_FILENO], STDOUT_FILENO);

		// replace process image with grep
		execlp("grep", "grep", arg_value[1], NULL);
		// "grep" should exit successfully here.
		// if the process image was replaced, the exit(1) will not be here
		// if the process image was not replaced, exit unsuccessfully
		_exit(1);
	}

	// Completely done with bucket1's stdin and bucket1's stdout
	close(bucket1[1]);

	// Wait for grep subprocess to cleanly exit
	wait(NULL);

	// Attempt a fork
	pid = fork();
	if (pid == 0) {
		// The child starts executing this part
		// Alias/redirect stdin in the child to be the data from the second bucket
		// (that had stdout piped into it)
		dup2(bucket2[STDIN_FILENO], STDIN_FILENO);
		close(bucket2[STDOUT_FILENO]);

		// Replace the child's process image with wc
		execlp("wc", "wc", "-l", NULL);
		// "wc" should exit successfully here.
		// if the process image was replaced, the exit(1) will not be here
		// if the process image was not replaced, exit unsuccessfully
		_exit(1);
	}

	// if the program hasn't died by this point, then exit successfully
	_exit(0);
}

