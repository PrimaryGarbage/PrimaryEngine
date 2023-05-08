#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include <functional>
#include <vector>

namespace prim
{
    template<class... T>
    class Event
    {
    private:
        std::vector<std::function<void(T...)>> subscribers;
    public:
        inline void invoke(T... args) const
        {
            for(auto& subscriber : subscribers)
            {
                subscriber(args...);
            }
        }

        inline void subscribe(std::function<void(T...)> callback)
        {
            subscribers.push_back(callback);
        }
    };
}

#endif // __EVENT_HPP__