#pragma once

template<typename T>
struct ListNode {
	T data;
	ListNode *next;
};

template<typename T>
struct List {
	using Node = ListNode<T>;
	Node *head;
	Node *tail;


	Node *CreateNode() {
		return new Node;
	}
	void DestroyNode(Node *node) {
		delete node;
	}

	Node *InsertFront(const T &value) {
		Node *old_head = head;
		head = CreateNode();
		head->data = value;
		head->next = old_head;
		if (!tail) tail = head;
		return head;
	}
	Node *InsertBack(const T &value) {
		if (!tail) return InsertFront(value);
		tail->next = CreateNode();
		tail = tail->next;
		tail->data = value;
		tail->next = nullptr;
		return tail;
	}

	void RemoveFront() {
		Node *old_head = head;
		head = head->next;
		if (!head) tail = nullptr;
		DestroyNode(old_head);

	}
	void RemoveBack() {
		if (!head) return;
		if (!head->next) {
			DestroyNode(head);
			head = nullptr;
			tail = nullptr;
			return;
		}
		Node *curr = head;

		while (curr->next->next) {
			curr = curr->next;
		}
		DestroyNode(curr->next);
		curr->next = nullptr;
	}

	List() {
		head = nullptr;
		tail = nullptr;
	}
	List(std::initializer_list<T> il) {
		head = nullptr;
		tail = nullptr;
		if (il.size() == 0)  return;
		for (auto it = std::rbegin(il); it != std::rend(il); ++it) {
			InsertFront(*it);
		}
	}

	bool Equals(const List<T> &other) {
		Node *curr0 = head;
		Node *curr1 = other.head;
		while (curr0 && curr1) {
			if (curr0->data != curr1->data) return false;
			curr0 = curr0->next;
			curr1 = curr1->next;
		}
		return !curr0 && !curr1;
	}

	Node *Find(const T &value) {
		Node *curr = head;
		if (curr->data == value) return curr;
		while (curr && curr->data != value) {
			curr = curr->next;
			if (curr && curr->data == value) return curr;
		}
		return nullptr;
	}
	void Remove(Node *node) {
		if (head == node) {
			Node *next = head->next;
			DestroyNode(head);
			head = next;
			return;
			
		}
		Node *curr = head;
		while (curr && curr != tail && curr->next != node) {
			curr = curr->next;
		}
		Node *after = curr->next->next;
		DestroyNode(curr->next);
		curr->next = after;
	}


};
