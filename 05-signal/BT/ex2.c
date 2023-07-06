#include <stdio.h>
#include <signal.h>
#include <unistd.h>
int count = 0, b = 0;
void signalHandler(int signal, siginfo_t *info, void *context)
{
    printf("Received signal: %d\n", signal);
    printf("Value: %d\n", info->si_value.sival_int);
    printf("Count: %d\n", count);
    printf("...............\n");

    count++;
}
void signalHandler_1(int signal, siginfo_t *info, void *context)
{
    printf("Received signal: %d\n", signal);
    printf("Value: %d\n", info->si_value.sival_int);
    printf("Count: %d\n", b);
    printf("...............\n");

    b++;
}

int main()
{
    // Initialization 2 value for mask bit
    sigset_t new_sig, old_sig;

    sigemptyset(&new_sig);
    sigemptyset(&old_sig);
    // Add SIGRTMIN+1 to new_sig
    sigaddset(&new_sig, SIGRTMIN + 1);
    sigaddset(&new_sig, SIGTRAP);
    struct sigaction sa, sp;

    // Register real time signal handler function and use SA_SIGINFO
    sa.sa_sigaction = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    // Register standard signal handler function and use SA_SIGINFO
    sp.sa_sigaction = signalHandler_1;
    sigemptyset(&sp.sa_mask);
    sp.sa_flags = SA_SIGINFO;
    // Register real time signal SIGRTMIN+1 by sigaction
    if (sigaction(SIGRTMIN + 1, &sa, NULL) == -1)
    {
        perror("sigaction");
        return 1;
    }
    // Register standard signal SIGTRAP by sigaction
    if (sigaction(SIGTRAP, &sp, NULL) == -1)
    {
        perror("sigaction");
        return 1;
    }

    // Send real time signal SIGRTMIN+1 with attached value
    union sigval value;
    value.sival_int = 125;

    printf("Block signal\n");
    sigprocmask(SIG_SETMASK, &new_sig, &old_sig);
    // Call standard signal with value
    for (int i = 0; i < 9; i++)
    {

        sigqueue(getpid(), SIGTRAP, value);
    }
    //
    // Call real time signal with value
    for (int i = 0; i < 3; i++)
    {

        sigqueue(getpid(), SIGRTMIN + 1, value);
    }
    // Signal has been sent waiting in line
    sleep(1);
    printf("Unblock signal\n");
    sleep(1);
    // Unblock signal
    sigemptyset(&new_sig);
    sigprocmask(SIG_SETMASK, &new_sig, &old_sig);
    // Loop
    while (1)
    {
        sleep(1);
    }

    return 0;
}
