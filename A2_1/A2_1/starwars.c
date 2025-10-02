#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Starting shield power level
int shield_power = 50;  

int main() {

    printf("Millennium Falcon: Initial shield power level: %d%%\n\n", shield_power);

    // Create 4 child processes - 4 different characters adjusting shield power
    int n = 4; // 4 character processes
    pid_t pid;

    // Luke increases the shield power by 25
    // Han increases the shield power by 20
    // Chewbacca increases the shield power by 30
    // Leia increases the shield power by 15    
    
    const char *characters[] = {"Luke", "Han", "Chewbacca", "Leia"};
    int shield_power_increases[] = {25, 20, 30, 15};

    for (int i = 0; i < n; i++) {
        pid = fork();

        if (pid < 0) {
            // Check if process creation failed
            printf("Fork failed.\n");
            return 1;
        } else if (pid == 0) {
            // Child process
            sleep(2);
            printf("%s: Adjusting shields...\n", characters[i]);
            int adjusted_shield_power = shield_power + shield_power_increases[i];
            printf("%s: Shield power level now at %d%%\n", characters[i], adjusted_shield_power);
            return 0;  // Child process exits
        }
    }

    // Make parent process wait for all child processes to complete
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    // Parent process reports final state
    printf("\nFinal shield power level on the Millennium Falcon: %d%%\n", shield_power);
    printf("\nMay the forks be with you!\n");
    return 0;
}