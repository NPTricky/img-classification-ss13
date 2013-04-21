#ifndef DERIVEDCREATOR_H
#define DERIVEDCREATOR_H

#include "Creator.h"

template <typename BaseType, typename DerivedType>
class DerivedCreator : public Creator<BaseType>
{
public:
    std::unique_ptr<BaseType> Create()
    {
        return std::unique_ptr<BaseType>(new DerivedType);
    }
};

#endif // DERIVEDCREATOR_H
