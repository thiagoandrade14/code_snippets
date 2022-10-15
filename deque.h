#include <memory>
#include <exception>

namespace cop3530 {

template <typename T>
class deque {
    
    struct node {
        node *next; // something to point to the next node
        node *previous; // something to point to the previous node
        T value;// something to hold a value of type T
		
		node() {
			next = nullptr;
			previous = nullptr;
		}
		~node() {
			next = nullptr;
			previous = nullptr;
		}
		node(const node& copyNode) {
			this->next = copyNode.next;
			this->previous = copyNode.previous;
			this->value = copyNode.value;			
		}
		
		node& operator=(const node& copyNode) {
			this->next = copyNode.next;
			this->previous = copyNode.previous;
			this->value = copyNode.value;
			return *this;
		}
		
    };

    node *head;// something to hold the head of the list
    node *tail; // something else?

    public:
        deque();
		~deque();
		deque(const deque& copyDeque);
		deque& operator=(const deque& copyDeque);
        void push_front(T data);
        void push_back(T data);
        void pop_front();
        void pop_back();
        T front();
        T back();
        size_t size();
        bool empty();
};

template <typename T>
deque<T>::deque() {
	head = nullptr;
	tail = nullptr;
}

template <typename T>
deque<T>::~deque() {
	delete head;
	delete tail;
}

template <typename T>
deque<T>::deque(const deque& copyDeque){
	this->tail = copyDeque.tail;
	this->head = copyDeque.head;
}

template <typename T>
deque<T>& deque<T>::operator=(const deque& copyDeque){
	this->tail = copyDeque.tail;
	this->head = copyDeque.head;
	return *this;
}



template <typename T>
void deque<T>::push_front(T data) {
	if (!this->empty()) {
		node *insertedNode = new node;
		insertedNode->next = head;
		insertedNode->previous = nullptr;
		insertedNode->value = data;
		head->previous = insertedNode;
		head = insertedNode;
	}
	else {
		node *insertedNode = new node;
		insertedNode->next = nullptr;
		insertedNode->previous = nullptr;
		insertedNode->value = data;
		head = insertedNode;
		tail = insertedNode;
	}
}

template <typename T>
void deque<T>::push_back(T data) {
	if (this->empty()) {
		node *insertedNode = new node;
		insertedNode->next = nullptr;
		insertedNode->previous = nullptr;
		insertedNode->value = data;
		head = insertedNode;
		tail = insertedNode;
	}
	else {
		node *insertedNode = new node;
		insertedNode->next = nullptr;
		insertedNode->previous = tail;
		insertedNode->value = data;
		tail->next = insertedNode;
		tail = insertedNode;
	}
}

template <typename T>
void deque<T>::pop_front() {
	if (!this->empty() ) {
		if (head->next != nullptr) {
			node *temp = head->next;
			temp->previous = nullptr;
			delete head;
			head = temp;
		}
		else {
			delete head;
			head = nullptr;
			tail = nullptr;
		}
				
	}
	else {
		throw std::runtime_error("");//throw exception
	}
}

template <typename T>
void deque<T>::pop_back() {
    if (!this->empty()) {
		if (tail->previous != nullptr) {
			node *temp = tail->previous;
			temp->next = nullptr;
			delete tail;
			tail = temp;		
		}
		else {
			delete tail;
			head = nullptr;
			tail = nullptr;			
		}
	}
	else {
		throw std::runtime_error(""); //throw exception
	}
}

template <typename T>
T deque<T>::front() {
    if (head != nullptr) {
		return head->value;
	}
	else {
		throw std::runtime_error("");//throw exception
	}
}

template <typename T>
T deque<T>::back() {
    if (tail != nullptr) {
		return tail->value;
	}
	else {
		throw std::runtime_error("");//throw exception
	}
}

template <typename T>
size_t deque<T>::size() {
	if (head != nullptr) {
		node *temp = head;
		unsigned int size = 0;
		while (temp != nullptr) {
			temp = temp->next;
			size++;
		}
		return size;
	}
	else {
		return 0;
	}
}

template <typename T>
bool deque<T>::empty() {
    return (head == nullptr);
}

}