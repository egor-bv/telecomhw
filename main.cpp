#include <iostream>

#include "linked_list.hpp"
using IntList = List<int>;


#include "utest.h"


UTEST(list, front_and_back) {
	IntList a{ 1, 2 };
	IntList b{ 2, 3 };
	a.InsertBack(3);
	b.InsertFront(1);
	ASSERT_TRUE(a.Equals(b));
}
UTEST(list, insert_back) {
	IntList a{ 1, 2, 3, 4, 5 };
	IntList b;
	b.InsertBack(1);
	b.InsertBack(2);
	b.InsertBack(3);
	b.InsertBack(4);
	b.InsertBack(5);
	ASSERT_TRUE(a.Equals(b));
}
UTEST(list, insert_front) {
	IntList a{ 5, 4, 3, 2, 1 };
	IntList b;
	b.InsertFront(1);
	b.InsertFront(2);
	b.InsertFront(3);
	b.InsertFront(4);
	b.InsertFront(5);
	ASSERT_TRUE(a.Equals(b));
}
UTEST(list, remove_front) {
	IntList a{ 1, 2, 3, 4, 5 };
	IntList b{ 3, 4, 5 };
	a.RemoveFront();
	a.RemoveFront();
	ASSERT_TRUE(b.Equals(a));
}

UTEST(list, remove_back) {
	IntList a{ 1, 2, 3, 4, 5 };
	IntList b{ 1, 2, 3 };
	a.RemoveBack();
	a.RemoveBack();
	ASSERT_TRUE(b.Equals(a));
}
UTEST(list, remove_back_all) {
	IntList a{ 1, 2, 3 };
	IntList b{};
	a.RemoveBack();
	a.RemoveBack();
	a.RemoveBack();
	ASSERT_TRUE(b.Equals(a));
}

UTEST(list, remove_front_all) {
	IntList a{ 1, 2, 3 };
	IntList b{};
	a.RemoveFront();
	a.RemoveFront();
	a.RemoveFront();
	ASSERT_TRUE(b.Equals(a));
}


UTEST(list, remove_all_then_insert) {
	IntList a{ 1, 2, 3 };
	a.RemoveFront();
	a.RemoveFront();
	a.RemoveFront();
	a.InsertBack(5);
	a.InsertBack(6);
	a.InsertFront(4);
	IntList b{4, 5, 6};
	ASSERT_TRUE(b.Equals(a));
}




UTEST(list, insert_empty) {
	IntList a{};
	IntList b{};
	a.InsertBack(111);
	b.InsertFront(111);
	ASSERT_TRUE(b.Equals(a));
	a.InsertBack(222);
	b.InsertBack(222);
	ASSERT_TRUE(b.Equals(a));

}
UTEST(list, find_and_remove_beginning) {
	IntList a{ 1, 2, 3 };
	auto *p = a.Find(1);
	a.Remove(p);
	IntList b{ 2, 3 };
	ASSERT_TRUE(b.Equals(a));
}
UTEST(list, find_and_remove_end) {
	IntList a{ 1, 2, 3 };
	auto *p = a.Find(3);
	a.Remove(p);
	IntList b{ 1, 2 };
	ASSERT_TRUE(b.Equals(a));
}
UTEST(list, find_and_remove_middle) {
	IntList a{ 1, 2, 3 };
	auto *p = a.Find(2);
	a.Remove(p);
	IntList b{ 1, 3 };
	ASSERT_TRUE(b.Equals(a));
}



#include "tokenizer.hpp"
UTEST(tokenizer, simple) {
	std::string example = "max(10, 20) * unknown / (111 * 2)";
	auto tokens = Tokenize(example);
	ASSERT_EQ(tokens.size(), 14);
}



UTEST_MAIN()