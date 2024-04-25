#pragma once
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <algorithm>

#define BUFFER_SIZE 1024
#define PORT 8080

// Define a circular buffer class
class CircularBuffer {
public:
	CircularBuffer(int size) : size_(size), head_(0), tail_(0), full_(false) {}

	bool Push(const char *data, int len) {
		std::lock_guard<std::mutex> lock(mutex_);
		if (full_) {
			return false;
		}
		int next_tail = (tail_ + 1) % size_;
		if (next_tail == head_) {
			full_ = true;
			return false;
		}
		memcpy(&buffer_[tail_], data, len);
		tail_ = next_tail;
		return true;
	}

	bool Pop(char *data, int &len) {
		std::lock_guard<std::mutex> lock(mutex_);
		if (head_ == tail_ && !full_) {
			return false;
		}
		len = std::min(len, size_ - head_);
		memcpy(data, &buffer_[head_], len);
		head_ = (head_ + len) % size_;
		full_ = false;
		return true;
	}

private:
	int size_;
	int head_;
	int tail_;
	bool full_;
	std::mutex mutex_;
	char buffer_[BUFFER_SIZE];
};

// Function to handle client connection
void handle_client(int client_socket, CircularBuffer &buffer) {
	char recv_buffer[BUFFER_SIZE];
	int bytes_received;

	while ((bytes_received = recv(client_socket, recv_buffer, BUFFER_SIZE, 0)) > 0) {
		// Process received data (e.g., store in circular buffer)
		if (!buffer.Push(recv_buffer, bytes_received)) {
			std::cerr << "Buffer full, dropping data\n";
		}

		// Send acknowledgement message
		const char *ack_message = "Message received\n";
		send(client_socket, ack_message, strlen(ack_message), 0);
	}

	if (bytes_received == 0) {
		std::cout << "Client disconnected\n";
	}
	else {
		std::cerr << "Error receiving data\n";
	}

	close(client_socket);
}

int main() {
	int server_socket, client_socket;
	struct sockaddr_in server_address, client_address;
	socklen_t client_address_size;

	// Create socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		std::cerr << "Failed to create socket\n";
		return 1;
	}

	// Set socket options (reuse address)
	int reuseaddr = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) == -1) {
		std::cerr << "Failed to set socket options\n";
		close(server_socket);
		return 1;
	}

	// Configure server address
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);

	// Bind socket to address
	if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
		std::cerr << "Failed to bind socket\n";
		close(server_socket);
		return 1;
	}

	// Listen for incoming connections
	if (listen(server_socket, 5) == -1) {
		std::cerr << "Failed to listen for connections\n";
		close(server_socket);
		return 1;
	}

	std::cout << "Server listening on port " << PORT << std::endl;
	CircularBuffer buffer(BUFFER_SIZE); // Create circular buffer

	while (true) {
		// Accept incoming connection
		client_address_size = sizeof(client_address);
		client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
		if (client_socket == -1) {
			std::cerr << "Failed to accept connection\n";
			continue;
		}

		std::cout << "Client connected: " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << std::endl;

		// Spawn a new thread to handle the client connection
		std::thread client_thread(handle_client, client_socket, std::ref(buffer));
		client_thread.detach(); // Detach the thread to avoid resource leaks
	}

	// Close the server socket (would never reach here in this example)
	close(server_socket);

	return 0;
}