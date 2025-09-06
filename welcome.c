#include <stdio.h>

// Single line comment 
/*Multi 
line
comments
*/

int main()
{
    char name[20];
    printf("What is your name: ");
    //scanf("%20s", name);
    fgets(name, sizeof(name), stdin);
    printf("Welcome to CSC 410 %s\n", name);
    return 0;
}