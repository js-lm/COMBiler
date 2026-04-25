#pragma once

#include <array>
#include <initializer_list>
#include <vector>

namespace utilities{

    template<typename Type, size_t Capacity>
    class StaticSet{
    private:
        std::array<Type, Capacity> data_{};
        size_t size_{0};

        // TODO: I can add a bitset to speed up the lookup, though it won’t be general anymore. Anyway, the size is so small that the entire array probably fits in the cache anyway. Too lazy :)
    
    public:
        StaticSet() = default;
        ~StaticSet() = default;

        StaticSet(std::initializer_list<Type> initializerList){
            for(const Type &value : initializerList){
                add(value);
            }
        }

        bool add(const Type &value){
            if(contains(value)) return false;
            if(size_ >= Capacity) return false;

            data_[size_] = value;
            size_++;

            return true;
        }

        bool remove(const Type &value){
            for(size_t index{0}; index < size_; index++){
                if(data_[index] != value) continue;

                // for(size_t shiftIndex{index + 1}; shiftIndex < size_; shiftIndex++){
                //     data_[shiftIndex - 1] = data_[shiftIndex];
                // }

                data_[index] = data_[size_ - 1];

                size_--;

                return true;
            }
            
            return false;
        }

        void clear(){ size_ = 0;}

        bool contains(const Type &value) const{
            for(size_t index{0}; index < size_; index++){
                if(data_[index] == value) return true;
            }

            return false;
        }

        bool empty() const{ return size_ == 0;}

        size_t size() const { return size_;}

        std::vector<Type> toVector() const{
            std::vector<Type> values{};
            values.reserve(size_);

            for(size_t index{0}; index < size_; index++){
                values.push_back(data_[index]);
            }

            return values;
        }

    };



} // namespace utilities