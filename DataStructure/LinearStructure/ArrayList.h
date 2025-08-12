#pragma once
#include "linear_list.h"

template<typename T>
class ArrayList : public LinearList<T> {
public:
    // 构造函数
    explicit ArrayList(size_t capacity = 10) : capacity_(capacity), size_(0) {
        data_ = allocate(capacity_);
    }

    // 析构函数
    ~ArrayList() {
        destroy_elements();
        deallocate(data_);
    }

    // 拷贝操作要实现一个深拷贝，不然会拷贝指针，导致两个变量指向同一个指针，出问题
    /* 适用于ArrayList b = a; */
    ArrayList(const ArrayList& other) : capacity_(other.capacity_), size_(other.size_) {
        data_ = allocate(capacity_);
        for (size_t i = 0; i < size_; ++i){
            new (&data_[i]) T(other.data_[i]);
        }
    }
    /*适用于ArrayList b; b = a;*/
    ArrayList& operator = (const ArrayList& other){
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
    // 得到数据本身size_
    size_t size() const override {return size_;}
    // 根据pos，得到数据
    T& get(size_t pos) const override {
        if (pos < 0 || pos >= size_){
            throw std::out_of_range("Index out of range");
        }
        return data_[pos];
    }
    // 根据数据，找到pos索引
    size_t find(const T& value) const override {
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] == value) {
                return i;
            }
        }
        return size_;
    }
    // 在数据末端插入数据
    


private:
    T *data_;
    size_t capacity_; // 表示这个数组内部预先分配的空间大小
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
    // 扩容
    void resize(int new_capacity) {
        if (new_capacity < size_) throw std::invalid_argument("new capacity less than size");
        T* new_data = static_cast<T*>(::operator new(sizeof(T) * new_capacity));
        // 逐个移动构造到新内存，调用移动构造
        for (int i = 0; i < size_; ++i) {
            new (&new_data[i]) T(std::move(data_[i]));
            data_[i].~T();  // 显式调用析构
        }
        // 释放旧内存
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }
};