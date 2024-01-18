#pragma once
#include <algorithm>
#include <iostream>

template<typename Type>
class ArrayPtr {
public:
	ArrayPtr() = default;
	explicit ArrayPtr(size_t size) {
		if (size == 0) {
			ptr_ = nullptr;
			return;
		}
		ptr_ = new Type[size]{};
	}
	~ArrayPtr() {
		delete[] ptr_;
	}
	ArrayPtr(Type* ptr) {
		ptr_ = ptr;
	}

	ArrayPtr& operator=(const ArrayPtr& o_ptr) {
		if (this != &o_ptr) {
			this->ptr_ = o_ptr.ptr_;

		}
		return *this;
	}
	ArrayPtr(const ArrayPtr& o_ptr) {
		if (this != &o_ptr) {
			this->ptr_ = o_ptr.ptr_;
		}
	}
	//=========move====
	ArrayPtr& operator=(ArrayPtr&& o_ptr) {
		if (this != &o_ptr) {
			std::swap(this->ptr_, o_ptr.ptr_);
		}
		return *this;
	}
	ArrayPtr(ArrayPtr&& o_ptr) {
		if (this != &o_ptr) {
			this->ptr_ = o_ptr.ptr_;
			o_ptr.ptr_ = nullptr;
		}
	}
	//=================
	Type& operator[](size_t index) noexcept {
		return ptr_[index];
	}

	const Type& operator[](size_t index) const noexcept {
		return ptr_[index];
	}
	Type* Get()const {
		return ptr_;
	}
	ArrayPtr& operator=(Type* ptr) {
		ptr_ = ptr;
		return *this;
	}
	void swap(ArrayPtr* other) {
		std::swap(ptr_, other->ptr_);
	}
private:
	Type* ptr_ = nullptr;

};