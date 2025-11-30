#include "nb_stack.h"
#include <cstring>
#include <cstdlib>

Node::Node() {
    data = NULL;
    next = NULL;
}

Node::~Node() {
    if (data) free(data);
}

Node::Node(const char c) {
    data = (char*)malloc(2);
    data[0] = c;
    data[1] = '\0';
    next = NULL;
}

Node::Node(const char* str) {
    int len = strlen(str);
    data = (char*)malloc(len + 1);
    strcpy(data, str);
    next = NULL;
}

char* Node::getData() { return data; }
Node* Node::getNext() { return next; }

void Node::setData(const char c) {
    if (data) free(data);
    data = (char*)malloc(2);
    data[0] = c;
    data[1] = '\0';
}

void Node::setData(const char* str) {
    if (data) free(data);
    int len = strlen(str);
    data = (char*)malloc(len + 1);
    strcpy(data, str);
}

void Node::setNext(Node* n) {
    next = n;
}

NodeBasedStack::NodeBasedStack() {
    top = NULL;
}

NodeBasedStack::NodeBasedStack(const NodeBasedStack& other) {
    top = NULL;
    Node* curr = other.top;
    Node* last = NULL;

    while (curr != NULL) {
        Node* newNode = new Node(curr->getData());
        if (top == NULL) {
            top = newNode;
        } else {
            last->setNext(newNode);
        }
        last = newNode;
        curr = curr->getNext();
    }
}

NodeBasedStack& NodeBasedStack::operator=(const NodeBasedStack& other) {
    if (this == &other) return *this;

    while (!isEmpty()) pop();

    top = NULL;
    Node* curr = other.top;
    Node* last = NULL;

    while (curr != NULL) {
        Node* newNode = new Node(curr->getData());
        if (top == NULL) {
            top = newNode;
        } else {
            last->setNext(newNode);
        }
        last = newNode;
        curr = curr->getNext();
    }
    return *this;
}

NodeBasedStack::~NodeBasedStack() {
    while (!isEmpty()) pop();
}

void NodeBasedStack::push(const char c) {
    Node* temp = new Node(c);
    temp->setNext(top);
    top = temp;
}

void NodeBasedStack::push(const char* str) {
    Node* temp = new Node(str);
    temp->setNext(top);
    top = temp;
}

bool NodeBasedStack::pop() {
    if (top == NULL) return false;
    Node* temp = top;
    top = top->getNext();
    delete temp;
    return true;
}

char* NodeBasedStack::getTop() const {
    if (top == NULL) return NULL;
    return top->getData();
}

bool NodeBasedStack::isEmpty() const {
    return top == NULL;
}

int NodeBasedStack::getSize() const {
    int count = 0;
    Node* curr = top;
    while (curr != NULL) {
        count++;
        curr = curr->getNext();
    }
    return count;
}
