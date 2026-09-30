#ifndef __IPRINTABLE_H__
#define __IPRINTABLE_H__

#include <iostream>
#include <algorithm>


namespace Core {
    /// @brief Allows for custom types to be logged to the console
    class IPrintable {
        friend std::ostream& operator<<(std::ostream& os, const IPrintable& obj)
        {
            obj.Print(os);
            return os;
        }

    public:
        IPrintable() = default;
        virtual ~IPrintable() = default;

    protected:
        virtual void Print(std::ostream& os) const = 0;
    };

    /// @brief Used to log out elements of a container
    /// @tparam Iterator represents iterator to the container
    /// @param start represents iterator to start from
    /// @param end represents iterator to end at
    template<typename Iterator>
    void PrintContents(Iterator start, Iterator end) {
        for (; start != end; ++start) {
            std::cout << *start << ' ';
        }
    }
}


#endif