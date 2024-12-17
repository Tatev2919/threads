#include <iostream>
#include <thread>
#include <vector>


void sum_calculator(const std::vector<int>& arr, int start, int end, int& result) {
    result = 0;
    for (int i = start; i < end ; ++i) {
        result += arr[i];
    }
}


int main() {
    
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8};
    
    int sum1 = 0, sum2 = 0;

    int mid = arr.size()/2;

    std::thread t1(sum_calculator,std::cref(arr), 0, mid, std::ref(sum1));
    std::thread t2(sum_calculator,std::cref(arr), mid, arr.size(), std::ref(sum2));
    
    t1.join();
    t2.join();
 
    int total_sum = sum1 + sum2;


    std::cout << "Sum of first half: " << sum1 << std::endl;
    std::cout << "Sum of second half: " << sum2 << std::endl;
    std::cout << "Total sum: " << total_sum << std::endl;

    printf("All threads have completed.\n");
    return 0;
}

