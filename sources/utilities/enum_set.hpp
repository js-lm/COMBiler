#pragma once

#include <magic_enum/magic_enum.hpp>

#include <bitset>
#include <initializer_list>
#include <type_traits>

namespace utilities{

    template<typename EnumKey>
    class EnumSet{
        static_assert(std::is_enum_v<EnumKey>, "EnumKey must be an enum");
    private:
        std::bitset<magic_enum::enum_count<EnumKey>()> data_{};

        static constexpr size_t indexOf(EnumKey key){
            return magic_enum::enum_index(key).value();
        }

    public:
        constexpr EnumSet() = default;
        ~EnumSet() = default;

        constexpr EnumSet(std::initializer_list<EnumKey> initializerList){
            for(EnumKey key : initializerList){
                set(key);
            }
        }

        constexpr bool operator[](EnumKey key) const{
            return contains(key);
        }

        constexpr void set(EnumKey key, bool value = true){
            data_.set(indexOf(key), value);
        }

        constexpr void reset(EnumKey key){
            data_.reset(indexOf(key));
        }

        constexpr void clear(){
            data_.reset();
        }

        constexpr void flip(EnumKey key){
            data_.flip(indexOf(key));
        }

        constexpr bool contains(EnumKey key) const{
            return data_.test(indexOf(key));
        }

        constexpr bool any() const noexcept{
            return data_.any();
        }

        constexpr bool all() const noexcept{
            return data_.all();
        }

        constexpr bool none() const noexcept{
            return data_.none();
        }

        constexpr size_t count() const noexcept{
            return data_.count();
        }

        constexpr size_t size() const noexcept{
            return magic_enum::enum_count<EnumKey>();
        }

        constexpr const auto &bits() const noexcept{
            return data_;
        }

        class iterator{
        private:
            size_t index_;
            EnumSet *set_;

        public:
            constexpr iterator(size_t index, EnumSet *set) : index_{index}, set_{set}{}

            constexpr std::pair<EnumKey, bool> operator*() const{
                auto key{magic_enum::enum_value<EnumKey>(index_)};
                return {key, set_->data_.test(index_)};
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
            const EnumSet *set_;

        public:
            constexpr const_iterator(size_t index, const EnumSet *set) : index_{index}, set_{set}{}

            constexpr std::pair<EnumKey, bool> operator*() const{
                auto key{magic_enum::enum_value<EnumKey>(index_)};
                return {key, set_->data_.test(index_)};
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
    };

} // namespace utilities