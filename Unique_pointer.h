#pragma once
struct default_deleter {
	template<typename T>
	void operator()(T* pointer) {
		delete pointer;
	}
};

template<typename T,typename Deleter=default_deleter>
class Unique_pointer
{
public:
	Unique_pointer(const Unique_pointer& upointer) = delete;
	Unique_pointer(Unique_pointer&& pointer) :ptr(pointer.ptr), deleter(std::move(pointer.deleter)) {
		pointer.ptr = nullptr;
	}
	Unique_pointer(T* pointer) :ptr(pointer) {
		deleter = default_deleter();
	}
	Unique_pointer(T* pointer, Deleter custom_deleter) :ptr(pointer), deleter(std::move(custom_deleter)) {}
	Unique_pointer& operator=(const Unique_pointer<T>& upointer) = delete;
	Unique_pointer& operator=(Unique_pointer<T>&& upointer) {
		if (&upointer != this) {
			if (ptr)deleter(ptr);
			ptr = upointer.ptr;
			upointer.ptr = nullptr;
		}
		return *this;
	}

	T& operator*() const {
		return *ptr;
	}
	T* operator->() const {
		return ptr;
	}

	friend bool operator==(const Unique_pointer<T>& it1, const Unique_pointer<T>& it2) {
		return it1.ptr == it2.ptr;
	}
	friend bool operator!=(const Unique_pointer<T>& it1, const Unique_pointer<T>& it2) {
		return it1.ptr != it2.ptr;
	}friend bool operator>=(const Unique_pointer<T>& it1, const Unique_pointer<T>& it2) {
		return it1.ptr >= it2.ptr;
	}
	friend bool operator<=(const Unique_pointer<T>& it1, const Unique_pointer<T>& it2) {
		return it1.ptr <= it2.ptr;
	}
	friend bool operator>(const Unique_pointer<T>& it1, const Unique_pointer<T>& it2) {
		return it1.ptr > it2.ptr;
	}
	friend bool operator<(const Unique_pointer<T>& it1, const Unique_pointer<T>& it2) {
		return it1.ptr < it2.ptr;
	}

	void reset(T* data) {
		if (ptr)deleter(ptr);
		ptr = data;
	}
	T* release() {
		if (ptr) {
			return ptr;
			ptr = nullptr;
		}
	}
	T* get() const {
		return ptr;
	}
	Deleter get_deleter() const {
		return deleter;
	}
	operator bool() {
		return ptr != nullptr;
	}


	~Unique_pointer() {
		if(ptr!=nullptr)
		deleter(ptr);
	}
private:
	T* ptr;
	Deleter deleter;
};

template<typename T,typename Deleter>
class Unique_pointer<T[], Deleter> {
public:
	Unique_pointer(const Unique_pointer& upointer) = delete;
	Unique_pointer(Unique_pointer&& pointer) :value(pointer.value), deleter(std::move(pointer.deleter)) {
		pointer.value = nullptr;
	}
	Unique_pointer(T* pointer) :value(pointer) {}
	Unique_pointer(T* pointer, Deleter custom_deleter) :value(pointer), deleter(std::move(custom_deleter)) {}
	Unique_pointer& operator=(const Unique_pointer<T>& upointer) = delete;
	Unique_pointer& operator=(Unique_pointer<T>&& upointer) {
		if (&upointer != this) {
			if (value)deleter(value);
			value = upointer;
			upointer.ptr = nullptr;
		}
		return *this;
	}

	T& operator[](int n) const {
		return value[n];
	}
	void reset(T* data) {
		if (value)deleter(value);
		value = data;
	}
	T* release() {
		if (value) {
			return value;
			value = nullptr;
		}
	}
	T* get() const {
		return value;
	}
	Deleter get_deleter() const {
		return deleter;
	}
	operator bool() {
		return value != nullptr;
	}

	~Unique_pointer() {
		if (value != nullptr)
			deleter(value);
	}
private:
	T* value;
	Deleter deleter;
};