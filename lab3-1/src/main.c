#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void quickSort(int arr[], int left, int right){
    int pivot = arr[(left + right) / 2];
    int i = left;
    int j = right;
    while (i <= j) {
        while (arr[i] < pivot) {
            i++;
        }
        while (arr[j] > pivot) {
            j--;
        }
        if (i <= j)
        {
            swap(&arr[i], &arr[j]);
            i++;
            j--;
        }
    }
    if (j > left)
        quickSort(arr, left, j);
    if (i < right)
        quickSort(arr, i, right);
}


int main() {
    int n;
    if(scanf("%d", &n) != 1){
        return 0;
    }
    if (n == 0)
    {
        return 0;
    }
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        return 1;
    }
    for (int i = 0; i < n; i++)
    {
        if(scanf("%d", &arr[i]) != 1){
            free(arr);
            return 0;
        }
    }
    quickSort(arr, 0, n - 1);

    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    free(arr);
    return 0;
}
