#pragma once

namespace RESIN {

template <typename T>
struct Property
{
    T value;

    Property() { }
    Property(const T& v) : value(v) { }

    //  Getter
    T& operator()()
    {
        return value;
    }

    //  Const getter
    const T& operator()() const
    {
        return value;
    }
    
    //  Setter
    void operator()(const T& newValue)
    {
        value = newValue;
    }
};

//  Chaining property
template <typename P, typename T>
struct CProperty
{
    P* parent;
    T value;

    CProperty(P* par) : parent((P*)(par)) { }
    CProperty(P* par, const T& v) : parent((P*)(par)), value(v) { }

    //  Getter
    T& operator()()
    {
        return value;
    }

    //  Const getter
    const T& operator()() const
    {
        return value;
    }

    //  Chaining setter
    P& operator()(const T& newValue)
    {
        value = newValue;
        return *parent;
    }
};

}

