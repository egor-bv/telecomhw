#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx; // Mutex for synchronization
int shared_var = 0; // Shared variable

void writer_thread() {
    for (int i = 0; i < 100; ++i) {
        std::lock_guard<std::mutex> lock(mtx); // Acquire lock before writing
        shared_var++;
        std::cout << "Writer: Updated shared variable to " << shared_var << std::endl;
    }
}

void reader_thread() {
    for (int i = 0; i < 100; ++i) {
        std::lock_guard<std::mutex> lock(mtx); // Acquire lock before reading
        std::cout << "Reader: Read shared variable: " << shared_var << std::endl;
    }
}

int main() {
    std::thread writer(writer_thread);
    std::thread reader(reader_thread);

    writer.join();
    reader.join();

    return 0;
}