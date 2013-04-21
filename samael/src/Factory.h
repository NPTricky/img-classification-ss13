#ifndef FACTORY_H
#define FACTORY_H

#include "Creator.h"

template <typename KeyType, typename BaseType>
class Factory
{
public:
    typedef std::map<KeyType, Creator<BaseType>*> CreatorRegistry;

    ~Factory()
    {
        CreatorRegistry::iterator iter = myCreatorRegistry.begin();
        while (iter != myCreatorRegistry.end())
        {
            delete (*iter).second;
            ++iter;
        }
    }

    void RegisterCreator(const KeyType& key, Creator<BaseType>* creator)
    {
        CreatorRegistry::iterator iter = myCreatorRegistry.find(key);
        if (iter != myCreatorRegistry.end())
            return;
        else
            myCreatorRegistry[key] = creator;
    }

    std::unique_ptr<BaseType> Create(const KeyType& key)
    {
        CreatorRegistry::iterator iter = myCreatorRegistry.find(key);
        if (iter == myCreatorRegistry.end())
            return nullptr;
        else
            return (*iter).second->Create();
    }

private:
    CreatorRegistry myCreatorRegistry;
};

#endif // FACTORY_H
