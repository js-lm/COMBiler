#pragma once

#include "base_action.hpp"

namespace actions{

    struct AddNotes : public Base{
    private:
        int page_{0};
        

    public:
        AddNotes() = default;
        virtual ~AddNotes() = default;

   


    };

} // namespace actions