#ifndef __IPRINTABLE_H__
#define __IPRINTABLE_H__

#include <iostream>
#include <algorithm>


namespace Core {
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

    template<typename Iterator>
    void PrintContents(Iterator start, Iterator end) {
        for (; start != end; ++start) {
            std::cout << *start << ' ';
        }
    }
}


#endif