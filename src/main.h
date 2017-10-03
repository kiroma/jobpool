#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

class job{
    public:
    std::atomic_flag taken = ATOMIC_FLAG_INIT;
    bool finished = false;
};

#endif // MAIN_H_INCLUDED
