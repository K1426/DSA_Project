//sort_algo
#include <vector>

struct Doc
{
    int docID;
    int size;
    int first;
    Doc() {}
    Doc(int a, int b, int c) {docID = a; size = b; first = c;}
};

int getMax(int a[], int n)
{
    int max = a[0];
    for (int i = 1; i < n; i++)
        if (a[i] > max)
            max = a[i];
    return max;
}

int getMin(int a[], int n)
{
    int max = a[0];
    for (int i = 1; i < n; i++)
        if (a[i] < max)
            max = a[i];
    return max;
}

void swap(Doc& a, Doc& b)
{
    Doc temp = a;
    a = b;
    b = temp;
}

void quickSort(std::vector<std::vector<int>>& a, int start, int end, int (*f)(std::vector<int>*))
{
    //ensure the numbers to be sorted are more than one
    if (start >= end) return;
    
    //select pivot element (here it is center element)
    int pivot = f(&a[(start + end) / 2]);
    int i = start - 1, j = end + 1;
    
    while (true)
    {
        //decrease i until larger or equal element is found
        do i++; while (f(&a[i]) < pivot);
        //decrease j until smaller or equal element is found
        do j--; while (f(&a[j]) > pivot);
        //if all elements have been traversed break
        if (i >= j) break;
        //swap the elements that were on wrong side
        a[i].swap(a[j]);
    }

    //do the same with array on left of pivot
    quickSort(a, start, j, f);
    //and then with array on right of pivot
    quickSort(a, j + 1, end, f);
}