#pragma once
template<typename T>
class Shared_pointer;
class Controller {
public:
	int shared_count = 0;
};
template<typename T>
class Deleter {
	friend class Shared_pointer<T>;
	virtual void operator()(T* pointer) {
		delete pointer;
	}
};
template<typename T>
class Shared_pointer
{
public:

	Shared_pointer(const Shared_pointer<T>& pointer) :ptr(pointer.ptr), deleter(pointer.deleter), control(pointer.control) {
		control->shared_count++;
	}
	Shared_pointer(Shared_pointer<T>&& pointer) :ptr(pointer.ptr), deleter(pointer.deleter), control(pointer.control) {
		pointer.ptr = nullptr;
	}
	Shared_pointer(T* pointer) :ptr(pointer) {
		deleter = Deleter<T>();
		control = new Controller();
		control->shared_count++;
	}
	Shared_pointer(T* pointer, Deleter<T> custom_deleter) :ptr(pointer), deleter(std::move(custom_deleter)) {
		control = new Controller();
		control->shared_count++;
	}
	Shared_pointer& operator=(const Shared_pointer<T>& pointer) {
		if (&pointer != this) {
			if (ptr) {
				control->shared_count--;
				if (control->shared_count==0) {
					deleter(ptr);
					delete control;
				}
			}
			ptr = pointer.ptr;
			deleter = pointer.deleter;
			control = pointer.control;
			control->shared_count++;
		}
		return *this;
	}
	Shared_pointer& operator=(Shared_pointer<T>&& pointer) {
		if (&pointer != this) {
			if (ptr) {
				control->shared_count--;
				if (control->shared_count == 0) {
					deleter(ptr);
					delete control;
				}
			}
			ptr = pointer.ptr;
			deleter = std::move(pointer.deleter);
			pointer.ptr = nullptr;
			control = pointer.control;
			pointer.control = nullptr;
		}
		return *this;
	}

	T& operator*() const {
		return *ptr;
	}
	T* operator->() const {
		return ptr;
	}

	friend bool operator==(const Shared_pointer<T>& it1, const Shared_pointer<T>& it2) {
		return it1.ptr == it2.ptr;
	}
	friend bool operator!=(const Shared_pointer<T>& it1, const Shared_pointer<T>& it2) {
		return it1.ptr != it2.ptr;
	}friend bool operator>=(const Shared_pointer<T>& it1, const Shared_pointer<T>& it2) {
		return it1.ptr >= it2.ptr;
	}
	friend bool operator<=(const Shared_pointer<T>& it1, const Shared_pointer<T>& it2) {
		return it1.ptr <= it2.ptr;
	}
	friend bool operator>(const Shared_pointer<T>& it1, const Shared_pointer<T>& it2) {
		return it1.ptr > it2.ptr;
	}
	friend bool operator<(const Shared_pointer<T>& it1, const Shared_pointer<T>& it2) {
		return it1.ptr < it2.ptr;
	}

	void reset(T* data) {
		if (ptr) {
			control->shared_count--;
			if (control->shared_count == 0) {
				deleter(ptr);
				delete control;
			}
		}
		ptr = data;
		control = new Controller();
		control->shared_count++;
	}
	T* get() const {
		return ptr;
	}
	Deleter<T> get_deleter() const {
		return deleter;
	}
	operator bool() const {
		return ptr != nullptr;
	}
	bool unique() const {
		return control->shared_count == 1;
	}
	int ptr_count() const {
		return control->shared_count;
	}
	~Shared_pointer() {
		if (ptr) {
			control->shared_count--;
			if (control->shared_count == 0) {
				deleter(ptr);
				delete control;
			}
		}
	}
private:
	T* ptr;
	Deleter<T> deleter;
	Controller* control;
};

