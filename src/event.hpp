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
        std::vector<std::pair<std::string, std::function<void(T...)>>> subscribers;
    public:
        inline void invoke(T... args) const
        {
            for(auto& subscriber : subscribers)
            {
                subscriber.second(args...);
            }
        }

        inline void subscribe(std::string key, std::function<void(T...)> callback)
        {
            subscribers.push_back({key, callback});
        }

        inline void unsubscribe(std::string key)
        {
            const auto iter = std::find_if(subscribers.begin(), subscribers.end(), [&key](const auto& pair) { return pair.first == key; });
            if(iter != subscribers.end())
            {
                subscribers.erase(iter);
            }
        }
    };
}

#endif // __EVENT_HPP__