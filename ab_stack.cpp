#include "ab_stack.h"
#include <cstring>

ArrayBasedStack::ArrayBasedStack(const int capacity)
{
	this->capacity = capacity;
	this->top = -1;
	this->stack = new char*[capacity];
	for (int i = 0; i < capacity; i++)
		stack[i] = NULL;
}

ArrayBasedStack::ArrayBasedStack(const ArrayBasedStack& other)
{
	this->capacity = other.capacity;
	this->top = other.top;
	this->stack = new char*[capacity];
	for (int i = 0; i < capacity; i++)
	{
		if (i <= top && other.stack[i] != NULL)
		{
			int len = std::strlen(other.stack[i]);
			stack[i] = new char[len + 1];
			std::strcpy(stack[i], other.stack[i]);
		}
		else
			stack[i] = NULL;
	}
}

ArrayBasedStack& ArrayBasedStack::operator = (const ArrayBasedStack& other)
{
	if (this == &other)
		return *this;

	for (int i = 0; i <= top; i++)
	{
		delete[] stack[i];
	}

	delete[] stack;

	this->capacity = other.capacity;
	this->top = other.top;
	this->stack = new char*[capacity];

	for (int i = 0; i < capacity; i++)
	{
		if (i <= top && other.stack[i] != NULL)
		{
			int len = std::strlen(other.stack[i]);
			stack[i] = new char[len + 1];
			std::strcpy(stack[i], other.stack[i]);
		}
		else
			stack[i] = NULL;
	}

	return *this;
}

ArrayBasedStack::~ArrayBasedStack()
{
	for (int i = 0; i <= top; i++)
	{
		delete[] stack[i];
	}
	delete[] stack;
	stack = NULL;
}

void ArrayBasedStack::push(const char c)
{
	char temp[2];
	temp[0] = c;
	temp[1] = '\0';

	push(temp);
}

void ArrayBasedStack::push(const char* str)
{
	if (top == capacity - 1)
		return;

	int len = std::strlen(str);
	char* s = new char[len + 1];
	std::strcpy(s, str);

	stack[++top] = s;
}

bool ArrayBasedStack::pop()
{
	if (top == -1)
		return false;

	delete[] stack[top];
	stack[top] = NULL;
	top--;
	return true;
}

char* ArrayBasedStack::getTop() const
{
	if (top == -1)
		return NULL;
	return stack[top];
}

bool ArrayBasedStack::isEmpty() const
{
	return top == -1;
}

int ArrayBasedStack::getSize() const
{
	return top + 1;
}
