#ifndef CREATOR_H
#define CREATOR_H

template <typename BaseType>
class Creator
{
public:
    ~Creator(){};
    virtual std::unique_ptr<BaseType> Create() = 0;
};

#endif // CREATOR_H
