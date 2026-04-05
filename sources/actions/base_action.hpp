#pragma once

namespace actions{

    struct Base{
    private:



    public:
        Base() = default;
        virtual ~Base() = default;




    };


    // struct BeginAction : public Base{
    // private:

    // public:
    //     BeginAction() = default;
    //     virtual ~BeginAction() = default;

    // };

    struct EndAction : public Base{
    public:
        EndAction() = default;
        virtual ~EndAction() = default;
    };

} // namespace actions