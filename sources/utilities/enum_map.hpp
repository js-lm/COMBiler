#pragma once

#include <magic_enum/magic_enum.hpp>

#include <array>
#include <optional>
#include <cassert>
#include <type_traits>

namespace utilities{

    template<typename EnumKey, typename ValueType>
    class EnumMap{
        static_assert(std::is_enum_v<EnumKey>, "EnumKey must be an enum");
    private:
        std::array<ValueType, magic_enum::enum_count<EnumKey>()> data_{};

    public:
        constexpr EnumMap() = default;
        ~EnumMap() = default;

        constexpr EnumMap(std::initializer_list<std::pair<EnumKey, ValueType>> initializerList){
            for(const auto [key, value] : initializerList){
                auto index{magic_enum::enum_index(key)};
                if(index.has_value()){
                    data_[index.value()] = value;
                }
            }
        }

        constexpr ValueType &operator[](EnumKey key){
            size_t index{magic_enum::enum_index(key).value()};
            return data_[index];
        }

        constexpr const ValueType &operator[](EnumKey key) const{
            size_t index{magic_enum::enum_index(key).value()};
            return data_[index];
        }

        constexpr size_t size() const noexcept{
            return magic_enum::enum_count<EnumKey>();
        }

        constexpr void clear() noexcept{ data_.fill(ValueType{});}

        class iterator{
        private:
            size_t index_;
            EnumMap *map_;
            
        public:
            constexpr iterator(size_t index, EnumMap *map) : index_{index}, map_{map}{}
            
            constexpr std::pair<EnumKey, ValueType&> operator*(){
                auto key{magic_enum::enum_value<EnumKey>(index_)};
                return {key, map_->data_[index_]};
            }
            
            constexpr iterator &operator++(){
                index_++;
                return *this;
            }
            
            constexpr bool operator!=(const iterator &other) const{
                return index_ != other.index_;
            }
        };

        class const_iterator{
        private:
            size_t index_;
            const EnumMap *map_;
            
        public:
            constexpr const_iterator(size_t index, const EnumMap *map) : index_{index}, map_{map}{}
            
            constexpr std::pair<EnumKey, const ValueType&> operator*() const{
                auto key{magic_enum::enum_value<EnumKey>(index_)};
                return {key, map_->data_[index_]};
            }
            
            constexpr const_iterator &operator++(){
                index_++;
                return *this;
            }
            
            constexpr bool operator!=(const const_iterator &other) const{
                return index_ != other.index_;
            }
        };

        constexpr iterator begin() noexcept{ return iterator{0, this};}
        constexpr const_iterator begin() const noexcept{ return const_iterator{0, this};}
        
        constexpr iterator end() noexcept{ return iterator{magic_enum::enum_count<EnumKey>(), this};}
        constexpr const_iterator end() const noexcept{ return const_iterator{magic_enum::enum_count<EnumKey>(), this};}

        constexpr auto valueBegin() noexcept{ return data_.begin();}
        constexpr auto valueBegin() const noexcept{ return data_.begin();}
        
        constexpr auto valueEnd() noexcept{ return data_.end();}
        constexpr auto valueEnd() const noexcept{ return data_.end();}

        struct ValueRange{
            typename std::array<ValueType, magic_enum::enum_count<EnumKey>()>::iterator beginIterator, endIterator;
            auto begin(){ return beginIterator;}
            auto end(){ return endIterator;}
        };

        struct ConstValueRange{
            typename std::array<ValueType, magic_enum::enum_count<EnumKey>()>::const_iterator beginIterator, endIterator;
            auto begin() const{ return beginIterator;}
            auto end() const{ return endIterator;}
        };

        constexpr ValueRange values() noexcept{ 
            return {data_.begin(), data_.end()}; 
        }
        
        constexpr ConstValueRange values() const noexcept{ 
            return {data_.begin(), data_.end()}; 
        }
    };

} // namespace utilities