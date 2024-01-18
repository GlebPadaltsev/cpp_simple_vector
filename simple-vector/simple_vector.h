#pragma once

#include <cassert>
#include <stdexcept>
#include <initializer_list>
#include "array_ptr.h"

class ReserveProxyObj {
public:
    explicit ReserveProxyObj(size_t capacity_to_reserve)
        :capacity_(capacity_to_reserve)
    {
    }

    size_t ReserveCapasity() {
        return capacity_;
    }

private:
    size_t capacity_;

};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
    ArrayPtr<Type> items_;
    size_t size_ = 0;
    size_t capacity_ = 0;
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;
    SimpleVector(ReserveProxyObj capacity_to_reserve) {
        Reserve(capacity_to_reserve.ReserveCapasity());
    }

    explicit SimpleVector(size_t size) {
        capacity_ = size;
        size_ = size;
        ArrayPtr<Type> tmp(size);
        items_ = std::exchange(tmp, nullptr);
        std::fill(items_.Get(), items_.Get() + size_, 0);

    }
    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) {
        // Напишите тело конструктора самостоятельно
        capacity_ = size;
        size_ = size;
        ArrayPtr<Type> tmp(size);
        items_ = std::exchange(tmp, nullptr);
        std::fill(items_.Get(), items_.Get() + size_, value);

    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) {
        // Напишите тело конструктора самостоятельно
        capacity_ = init.size();
        size_ = init.size();
        ArrayPtr<Type> tmp(size_);
        items_ = std::exchange(tmp, nullptr);

        int index = 0;
        for (const Type& val : init) {
            *(items_.Get() + index++) = val;
        }
    }
    SimpleVector(const SimpleVector& other) {
        // Напишите тело конструктора самостоятельно
        if (this != &other) {
            capacity_ = other.capacity_;
            size_ = other.size_;
            Type* tmp = new Type[capacity_];
            std::copy(other.items_.Get(), other.items_.Get() + capacity_, tmp);
            items_ = tmp;
            tmp = nullptr;
        }
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        // Напишите тело конструктора самостоятельно
        if (this != &rhs) {
            capacity_ = rhs.capacity_;
            size_ = rhs.size_;
            Type* tmp = new Type[capacity_];
            std::copy(rhs.items_.Get(), rhs.items_.Get() + capacity_, tmp);
            items_ = tmp;
            tmp = nullptr;
        }
        return *this;
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        // Напишите тело самостоятельно
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        // Напишите тело самостоятельно
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return (size_ == 0) ? true : false;
    }

    void PopBack() noexcept {
        if (size_ != 0) {
            --size_;
        }
    };

    void PushBack(const Type& value) {
        if (size_ == capacity_) {
            Reserve(capacity_ * 2 + 1);
        }
        ++size_;
        (*this)[size_ - 1] = value;
    };
    Iterator Erase(ConstIterator pos) {
        assert(pos >= begin() && pos < end());

        std::move(const_cast<Iterator>(pos) + 1, end(), const_cast<Iterator>(pos));
        --size_;
        //----------------]
        return const_cast<Iterator>(pos);

    }
    //================================================MOVE===================================
    Iterator Insert(ConstIterator pos, Type&& value) {
        assert(pos >= begin() && pos <= end());
        if (capacity_ == 0) {
            PushBack(std::move(value));
            return begin();
        }
        if (size_ == capacity_) {
            auto distance_to_new_pos = pos - begin();
            Reserve(capacity_ * 2);
            pos = const_cast<Iterator>(std::next(begin(), distance_to_new_pos));
        }
        std::move_backward(const_cast<Iterator>(pos), end(), end() + 1);
        items_[pos - items_.Get()] = std::move(value);
        ++size_;
        return  const_cast<Iterator>(pos);
    }

    void PushBack(Type&& value) {
        if (size_ == capacity_) {
            Reserve(capacity_ * 2 + 1);
        }
        ++size_;
        (*this)[size_ - 1] = std::move(value);
    };
    SimpleVector& operator=(SimpleVector&& other) noexcept {
        if (this != &other) {
            size_ = std::exchange(other.size_, 0);
            capacity_ = std::exchange(other.capacity_, 0);
            items_ = std::exchange(other.items_, nullptr);
        }
        return *this;
    }

    SimpleVector(SimpleVector&& other) noexcept {
        if (this != &other) {
            size_ = std::exchange(other.size_, 0);
            capacity_ = std::exchange(other.capacity_, 0);
            items_ = std::exchange(other.items_, nullptr);
        }
    }
    //=========================================================================
    Iterator Insert(ConstIterator pos, const Type& value) {

        assert(pos >= begin() && pos <= end());
        if (capacity_ == 0) {
            PushBack(value);
            return begin();
        }
        if (size_ == capacity_) {
            auto distance_to_new_pos = pos - begin();
            Reserve(capacity_ * 2);
            pos = const_cast<Iterator>(std::next(begin(), distance_to_new_pos));
        }
        std::copy_backward(const_cast<Iterator>(pos), end(), end() + 1);
        items_[pos - items_.Get()] = value;
        ++size_;
        return  const_cast<Iterator>(pos);
    }

    void swap(SimpleVector& other) noexcept {
        items_.swap(&other.items_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        // Напишите тело самостоятельно
        assert(index < size_);
        return  *(items_.Get() + index);
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        // Напишите тело самостоятельно
        assert(index < size_);
        return  *(items_.Get() + index);

    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        // Напишите тело самостоятельно
        return  (index < size_) ? *(items_.Get() + index) : throw std::out_of_range("out_of_range");
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        // Напишите тело самостоятельно
        return  (index < size_) ? *(items_.Get() + index) : throw std::out_of_range("out_of_range");
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        // Напишите тело самостоятельно
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        // Напишите тело самостоятельно
        if (new_size == size_)return;
        if (new_size <= capacity_) {
            size_ = new_size;
            std::generate(end(), (items_.Get() + new_size), []() {return Type(); });
        }

        else {

            capacity_ = new_size;
            Type* tmp_copy = new Type[capacity_];

            std::move(items_.Get(), items_.Get() + size_, tmp_copy);
            std::generate(tmp_copy + size_, (tmp_copy + new_size), []() {return Type(); });

            size_ = new_size;
            items_ = std::exchange(tmp_copy, nullptr);
            delete tmp_copy;
        }
    }
    void Reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }
        capacity_ = new_capacity;
        Type* tmp_copy = new Type[capacity_];
        std::move(items_.Get(), items_.Get() + size_, tmp_copy);
        std::fill(tmp_copy + size_, tmp_copy + capacity_, 0);
        items_ = std::exchange(tmp_copy, nullptr);

        delete tmp_copy;
    }
    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        // Напишите тело самостоятельно
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        // Напишите тело самостоятельно
        return items_.Get() + size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        // Напишите тело самостоятельно
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        // Напишите тело самостоятельно
        return items_.Get() + size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        // Напишите тело самостоятельно
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        // Напишите тело самостоятельно
        return items_.Get() + size_;
    }

};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) ? true : false;
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !(lhs == rhs) ? true : false;
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) ? true : false;
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return (lhs < rhs || lhs == rhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !(lhs < rhs || lhs == rhs);
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !(lhs < rhs);
}