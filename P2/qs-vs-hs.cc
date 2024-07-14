//David Pérez Sampedro - 48774775D
/*
ADA. 2023-24
Práctica 2:"Empirical analysis by means of program-steps account of two sorting algorithms: Middle-Quicksort and Heapsort."
*/
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <iomanip>

using namespace std;

double STEPS_qs;
double STEPS_hs;

//--------------------------------------------------------------
// Middle Quick Sort
void 
middle_QuickSort(int * v, long left, long right){
    long i,j;
    int pivot; 
	if (left<right){
		i=left; j=right;
		pivot=v[(i+j)/2];
		do{
			while (v[i]<pivot) {
                i++;
                STEPS_qs++;
            }
			while (v[j]>pivot) { 
                j--;
                STEPS_qs++;
            }
			if (i<=j){ 
				swap(v[i], v[j]);
				i++;
                j--;
			}
            STEPS_qs++;
		}while (i<=j);
		if (left < j) {
            middle_QuickSort(v,left,j);
        }
		if (i < right) {
            middle_QuickSort(v,i,right);
        }
	}
    STEPS_qs++;
}

//--------------------------------------------------------------                
//HEAPSORT
// Procedure sink used by Heapsort algorith
// Sink an element (indexed by i) in a tree to keep the heap property. n is the size of the heap. 

void sink(int *v, size_t n, size_t i)
{
    size_t largest;
    size_t l, r; //left and right child

    do{
        largest = i;  // Initialize largest as root
        l = 2*i + 1;  // left = 2*i + 1
        r = 2*i + 2;  // right = 2*i + 2
    
        // Is left child larger than root?
        if (l < n && v[l] > v[largest]) {
            largest = l;
        }

        // Is right child larger than largest so far
        if (r < n && v[r] > v[largest]) {
            largest = r;
        }
    
        // If largest is still root then the process is done
        if (largest == i) break;
        
        // If not, swap new largest with current node i and repeat the process with childs. 
        swap(v[i], v[largest]);
        i=largest;  
        STEPS_hs++;
   } while (true);
   STEPS_hs++;
}
 
//--------------------------------------------------------------                
// Heapsort algorithm (ascending sorting)
void heapSort(int *v, size_t n)
{
    // Build a MAX-HEAP with the input array
    for (size_t i = n / 2 - 1; i >= 0; i--){
        sink(v, n, i);
        STEPS_hs++;
        if (i==0) break; //as size_t is unsigned type
	}	

  	// A this point we have a HEAP on the input array, let's take advantage of this to sort the array:
    // One by swap the first element, which is the largest (max-heap), with the last element of the vector and rebuild heap by sinking the new placed element on the begin of vector.  
    for (size_t i=n-1; i>0; i--)
    {
		// Move current root to the end.
        swap(v[0], v[i]);
        STEPS_hs++; 
 		// Now largest element is at the end but do not know if the element just moved to the beginning is well placed, so a sink process is required.
		// Note that, at the same time, the HEAP size is reduced one element by one.
        sink(v, i, 0);
		// The procedure ends when HEAP has only one element, the lower of the input array
    }
}

//--------------------------------------------------------------

int main() {

    srand(0);

    cout << "#QUICKSORT VERSUS HEAPSORT."
        << endl
        << "#Average processing Msteps (millions of program steps)"
        << endl
        << "#Number of samples (arrays  of integer): 30"
        << endl << endl
        << "#                RANDOM ARRAYS         SORTED ARRAYS      REVERSE SORTED ARRAYS"
        << endl
        << "#             -------------------   -------------------   ---------------------"
        << endl
        << "#    Size     QuickSort  HeapSort   QuickSort  HeapSort   QuickSort    HeapSort"
        << endl
        << "#=============================================================================="
        << endl;

        for(int exp = 15; exp <= 20; exp++) {
            size_t size = size_t(pow(2, exp));
            int *v1 = new int[size];
            int *v2 = new int[size];
            if(!v1 || !v2) {
                cerr << "Error, not enough memory!" << endl;
                exit(EXIT_FAILURE);
            }

            cout << std::setw(9) << size << std::setw(5) << std::flush;

            STEPS_qs = 0;
            STEPS_hs = 0;
            for(int i = 0; i < 30; i++) {
                for(size_t j = 0; j< size; j++) {
                    v1[j] = rand();
                    v2[j] = v1[j];
                }

                middle_QuickSort(v1, 0, size - 1);
                heapSort(v2, size);
            }
            double random_qs = (STEPS_qs / 30) / 1000000;
            double random_hs = (STEPS_hs / 30) / 1000000;

            STEPS_qs = 0;
            STEPS_hs = 0;
            for(int i = 0; i < 30; i++) {
                for(size_t j = 0; j< size; j++) {
                    v1[j] = j;
                    v2[j] = v1[j];
                }

                middle_QuickSort(v1, 0, size - 1);
                heapSort(v2, size);
            }
            double sorted_qs = (STEPS_qs / 30) / 1000000;
            double sorted_hs = (STEPS_hs / 30) / 1000000;

            STEPS_qs = 0;
            STEPS_hs = 0;
            for(int i = 0; i < 30; i++) {
                for(size_t j = 0; j < size; j++) {
                    v1[j] = size - 1 - j;
                    v2[j] = v1[j];
                }

                middle_QuickSort(v1, 0, size - 1);
                heapSort(v2, size);
            }
            double reverseSorted_qs = (STEPS_qs / 30) / 1000000;
            double reverseSorted_hs = (STEPS_hs / 30) / 1000000;

            std::cout << std::fixed << std::setprecision(3);

            cout << std::setw(12) << random_qs << std::setw(10) << random_hs 
            << std::setw(12) << sorted_qs << std::setw(10) << sorted_hs 
            << std::setw(12) << reverseSorted_qs << std::setw(12) << reverseSorted_hs << endl;

            delete[] v1;
            delete[] v2;
        }
    
    return 0;
}