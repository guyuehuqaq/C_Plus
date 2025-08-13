#pragma once
#include "linear_list.h"

template<typename T>
class ArrayList : public LinearList<T> {
public:
    using iterator = T*;  // 迭代器类型
    using const_iterator = const T*;

    // 构造函数
    explicit ArrayList(size_t capacity = 10) : capacity_(capacity), data_(allocate(capacity_)), size_(0) {}

    // 析构函数
    ~ArrayList() {
        destroy_elements();
        deallocate(data_);
    }

    iterator begin() { return data_; }
    iterator end() { return data_ + size_; }
    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ + size_; }

    // 拷贝操作要实现一个深拷贝，不然会拷贝指针，导致两个变量指向同一个指针，出问题
    /* 拷贝构造，适用于ArrayList b = a */
    ArrayList(const ArrayList& other) : data_(allocate(other.capacity_)), capacity_(other.capacity_), size_(0) {;
        for (size_t i = 0; i < size_; ++i){
            new (&data_[i]) T(other.data_[i]);
        }
        ++size_;
    }

    /* 拷贝构造，适用于ArrayList b; b = a; */
    ArrayList& operator=(const ArrayList& other){
        if (this == &other) return *this;
        // 先析构旧元素，若容量不够则重新分配
        if (capacity_ < other.size_) {
            destroy_elements();
            deallocate(data_);
            capacity_ = other.capacity_;
            data_ = allocate(capacity_);
        } else {
            destroy_elements();
        }
        size_ = other.size_;
        for (size_t i = 0; i < size_; ++i) {
            new (&data_[i]) T(other.data_[i]);
        }
        return *this;
    }

    /* 移动构造，适用于ArrayList b = std::move(a) */
    ArrayList(ArrayList&& other) noexcept : data_(other.data_), capacity_(other.capacity_), size_(other.size_){
        other.data_ = nullptr;  // noexcept:说明这个操作不会抛异常; 把对方的指针置空，防止析构时释放内存两次
        other.capacity_ = 0;
        other.size_ = 0;
    }

    /* 移动构造，适用于ArrayList b; b = std::move(a); */
    ArrayList& operator=(ArrayList&& other) noexcept {
        if (this != &other) {        // 保护自己避免自赋值
            destroy_elements();
            deallocate(data_);
            data_ = other.data_;
            capacity_ = other.capacity_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.capacity_ = 0;
            other.size_ = 0;
        }
        return *this;
    }


    // 得到数据本身size_
    // 大小与容量
    size_t size() const override { return size_; }
    size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    void clear() {
        destroy_elements();
    }

    // 根据pos，得到数据
    T& get(size_t pos) const override {
        if (pos < 0 || pos >= size_){
            throw std::out_of_range("Index out of range");
        }
        return data_[pos];
    }
    T& operator[](size_t index) {
        return data_[index];
    }
    const T& operator[](size_t index) const {
        return data_[index];
    }
    // 带范围检查的访问
    T& at(size_t index) {
        if (index >= size_) throw std::out_of_range("Index out of range");
        return data_[index];
    }

    const T& at(size_t index) const {
        if (index >= size_) throw std::out_of_range("Index out of range");
        return data_[index];
    }

    // 根据数据，找到pos索引
    iterator find(const T& value) {
        for (iterator it = begin(); it != end(); ++it) {
            if (*it == value) return it;
        }
        return end();
    }


    // 扩容
    void reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) return;
        T* new_data = allocate(new_capacity);
        for (size_t i = 0; i < size_; ++i) {
            new (&new_data[i]) T(std::move(data_[i]));
            data_[i].~T();
        }
        deallocate(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }

    // 改变size大小
    void resize(size_t new_size) {
        if (new_size < size_) {
            // 缩小，析构多余元素
            for (size_t i = new_size; i < size_; ++i) {
                data_[i].~T();
            }
        } else if (new_size > size_) {
            // 扩大，确保容量足够
            if (new_size > capacity_) {
                reserve(new_size);
            }
            // 构造新元素，默认构造
            for (size_t i = size_; i < new_size; ++i) {
                new (&data_[i]) T();
            }
        }
        size_ = new_size;
    }

    // 在数据末端插入数据（左值）
    void push_back(const T& value){
        if (size_ == capacity_){reserve(capacity_ == 0 ? 1 : capacity_ * 2);}
        new (&data_[size_]) T(value);
        ++size_;
    }

    // 在数据末端插入数据（右值）
    void push_back(T&& value){
        if (size_ == capacity_){reserve(capacity_ == 0 ? 1 : capacity_ * 2);}
        new (&data_[size_]) T(std::move(value));
        ++size_;
    }

    // 插入数据
    iterator insert(iterator pos, const T& value) {
        size_t index = pos - data_;
        if (size_ >= capacity_) resize(capacity_ * 2);
        pos = data_ + index;
        new (data_ + size_) T(std::move(data_[size_ - 1]));
        for (size_t i = size_ - 1; i > index; --i) {
            data_[i] = std::move(data_[i - 1]);
        }
        data_[index].~T();
        new (data_ + index) T(value);
        ++size_;
        return data_ + index;
    }

    // 删除数据
    iterator erase(iterator pos) {
        if (pos + 1 != end()) {
            std::move(pos + 1, end(), pos);
        }
        --size_;
        (data_ + size_)->~T();
        return pos;
    }

private:
    size_t capacity_; // 表示这个数组内部预先分配的空间大小
    T *data_;
    size_t size_;  // 表示当前数组中实际存储的大小

    // 只分配内存，不构造对象
    T* allocate(size_t capacity) {
        return static_cast<T*>(::operator new(sizeof(T) * capacity));
    }
    // 释放内存，但不调用析构函数
    void deallocate(T* ptr) {
        ::operator delete(ptr);
    }
    // 析构所有已构造元素
    void destroy_elements() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }
};