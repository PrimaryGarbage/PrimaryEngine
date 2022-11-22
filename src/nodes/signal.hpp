#ifndef __SIGNAL_HPP__
#define __SIGNAL_HPP__

#include <string>
#include <functional>
#include <algorithm>

namespace prim
{
    template <class... T>
    class Signal
    {
        typedef std::function<void(T...)> callback_type;
        typedef std::pair<std::string, callback_type> callback_pair;

    private:
        std::vector<callback_pair> callbacks;
    public:
        inline void emit(T&&... args)
        {
            for(callback_pair& pair : callbacks)
                pair.second(std::forward<T>(args)...);
        }

        inline void connect(std::string connectionName, callback_type function)
        {
            callbacks.push_back({connectionName, function});
        }

        inline void disconnect(std::string connectionName)
        {
            auto iter = std::find_if(callbacks.begin(), callbacks.end(), [&connectionName](const callback_pair& pair){ return pair.first == connectionName; });
            if(iter != callbacks.end()) callbacks.erase(iter);
        }
    };

}


#endif // __SIGNAL_HPP__