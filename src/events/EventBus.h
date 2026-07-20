#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>

using SubscriptionId = uint32_t;

class EventBus
{
public:
    EventBus() = default;
    ~EventBus() = default;

    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;
    EventBus(EventBus&&) = delete;
    EventBus& operator=(EventBus&&) = delete;

    //Subscribe
    template<typename EventType>
    SubscriptionId Subscribe(std::function<void(EventType&)> callback)
    {
        SubscriptionId id = _nextId++;
        std::type_index type = std::type_index(typeid(EventType));

        auto voidCallback = [callback](void* pEvent) { callback(*static_cast<EventType*>(pEvent)); };
        _subscriptions[type].push_back({ id, voidCallback });
        _types.emplace(id, type);

        return id;
    }

    void Unsubscribe(SubscriptionId id)
    {
        auto typeIt = _types.find(id);
        if (typeIt == _types.end())
            return;

        auto& typeSub = _subscriptions[typeIt->second];
        auto isMatch = [id](const Subscription& sub) { return sub.id == id; };
        auto subIt = std::find_if(typeSub.begin(), typeSub.end(), isMatch);

        if (subIt != typeSub.end())
            typeSub.erase(subIt);

        _types.erase(typeIt);
    }

    template<typename EventType>
    void Emit(EventType event)
    {
        auto it = _subscriptions.find(std::type_index(typeid(EventType)));
        if (it == _subscriptions.end())
            return;

        for (auto& subscription : it->second)
            subscription.callback(&event);
    }

private:
    struct Subscription
    {
        SubscriptionId id{ 0 };
        std::function<void(void*)> callback{};
    };

    std::unordered_map<std::type_index, std::vector<Subscription>> _subscriptions{};
    std::unordered_map<SubscriptionId, std::type_index> _types{};

    SubscriptionId _nextId{ 1 }; //Zero reserved as invalid
};