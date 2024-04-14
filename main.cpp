#include <iostream>


template<typename Functor>
void invert_bits_masked(char *bytes, size_t num_bytes, Functor ith_byte_mask) {
	for (size_t i = 0; i < num_bytes; ++i) {
		bytes[i] ^= ith_byte_mask(i);
	}
}

template<typename Iterator>
void invert_bits_at_positions(char *bytes, size_t num_bytes, Iterator begin, Iterator end) {
	for (Iterator it = begin; it < end; ++it) {
		size_t bit_idx = *it;
		size_t byte_idx = bit_idx >> 3;
		if (byte_idx < num_bytes) {
			bytes[byte_idx] ^= 1 << (bit_idx & 7);
		}
	}
}

void invert_bits_at_positions(char *bytes, size_t num_bytes, std::initializer_list<size_t> il) {
	invert_bits_at_positions(bytes, num_bytes, il.begin(), il.end());
}

int main(int argc, char **argv) {
	char example[] = "Helmg, World)";
	size_t num_bytes = sizeof(example) - 1;
	
	invert_bits_masked(example, num_bytes, [](auto i) { return 0xff; });
	invert_bits_masked(example, num_bytes, [](auto i) { return 0x0f; });
	invert_bits_masked(example, num_bytes, [](auto i) { return 0xf0; });
	
	invert_bits_at_positions(example, num_bytes, { 35, 24, 99 });

	// prints "Hello, World!"
	std::cout << example;
}