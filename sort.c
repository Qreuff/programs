void bubble_Sort(int arr[], int n, int *M, int *C) {
    int i, j, temp;
    *M = 0;
    *C = 0;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            (*C)++;
            if (arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
                (*M)+=3;
            }
        }
    }
}
int main(){
    int a[] = {4,2};
    int size = sizeof(a) / sizeof(a[0]);
    bubble_Sort(a,size)
    printf(""):
    for(int i ==0; i<n; i++){
        printf("%d", a[i]);
    }
    printf("\n")
}
