#include <stdio.h>
#include <limits.h>
#define MAX_SIZE 10
#include <stdbool.h>

int main() {
    int N;
    bool flag = true;
    long long int array[10];
    long long sum = 0;
    if (scanf("%d", &N) != 1) {
        printf("bad input\n");
        return 0;
    }
    if (N < 1 || N > 10) {
        printf("bad input\n");
        return 0;
    }

    for (int i = 0; i < N; i++) {
        if (scanf("%lld", &array[i]) != 1) {
            printf("bad input\n");
            return 0;
        }

        if (array[i] < INT_MIN || array[i] > INT_MAX) {
            printf("bad input\n");
            return 0;
        }

        char c = getchar();
        if (c == '\n') {
            if (i < N - 1) {
                printf("bad input\n");
                flag = false;
                break;
            }
        }

    }

    if (flag == true){
        for (int i = 0; i < N; i++) {
            sum += array[i];

            if (sum < INT_MIN || sum > INT_MAX) {
                printf("overflow\n");
                return 0;
            }
        }
    }
    if (flag == true) {
        printf("%lld\n", sum);
    }
    return 0;
}




