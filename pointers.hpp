#pragma once
#include <iostream>

void Print4thElement(int *ptr) {
	std::cout << *(ptr + 3) << "\n";
}

int main(int argc, char **argv) {
	int a[10];
	Print4thElement(a);
}