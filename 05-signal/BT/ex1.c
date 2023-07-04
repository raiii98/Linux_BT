#include <stdio.h>
#include <signal.h> // Include functions signal
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

// Initialization signalhandler1
void signalhandler1()
{
    printf("I am signalhandler1\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char argv)
{
    sigset_t new_sig, old_sig;
    if (signal(SIGINT, signalhandler1) < 0) // register signalhandler1 for SIGINT
    {
        fprintf(stderr, "Cannot handler SIGINT\n"); // print error to global variable stderr
    }
    sigemptyset(&new_sig);                                 // set empty for new_sig
    sigemptyset(&old_sig);                                 // set empty for old_sig
    sigaddset(&new_sig, SIGINT);                           // add SIGINT to new_sig
    if (sigprocmask(SIG_SETMASK, &new_sig, &old_sig) == 0) // current bit-mask save to old_sig, new_sig overwrite to current bit-mask
    {
        sigprocmask(SIG_SETMASK, NULL, &old_sig); // put out current bit-mask
        for (int j = 1; j <= NSIG; j++)           // print ID of blocked signal
        {
            if (sigismember(&old_sig, j) == 1)
            {

                printf("ID of blocked signal: %d\n", j);
            }
        }
        printf("Signal Bit-Mask: ");
        for (int i = 1; i <= NSIG; i++) // print Signal Bit-Mask
        {
            if (sigismember(&old_sig, i) == 1)
            {

                printf("%d", 1);
            }
            else
            {
                printf("%d", 0);
            }
        }
        printf("\n ");
    }
    while (1)
    {
        /* code */
    }
}