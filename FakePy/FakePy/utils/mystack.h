#pragma once
#ifndef __MYSTACK_H__
#define __MYSTACK_H__

#include "stdafx.h"
template<typename T>
class Stack {
private:
	int top;
	int capacity;
	T* arr;
public:
	Stack(unsigned int cap) {
		if (capacity < 0) {
			printf("Capacity should be greater than zero");
			return;
		}
		arr = (T*)malloc(sizeof(T) * capacity);
		top = -1;
		capacity = cap;
	}

	bool empty() {
		return top == -1;
	}
	T peek() {
		return arr[top];
	}
	T pop() {
		if (!empty())
			return arr[top--];
		return NULL;
	}

	void push(T item) {
		if (top == capacity - 1) {
			printf("FULL stack\n");
			return;
		}
		arr[++top] = item;
	}
};


#endif