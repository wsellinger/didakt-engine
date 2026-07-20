#include "events/EventBus.h"

#include <catch2/catch_test_macros.hpp>

namespace
{
    struct TestEventA
    {
        int value;
    };

    struct TestEventB
    {
        int otherValue;
    };
}

TEST_CASE("Events.EventBus.Subscribe_ReceivesEmittedEvent", "[EventBus]")
{
    EventBus bus;
    bool wasCalled = false;
    int receivedValue = 0;

    bus.Subscribe<TestEventA>([&](TestEventA& e)
        {
            wasCalled = true;
            receivedValue = e.value;
        });

    TestEventA event{ 42 };
    bus.Emit(event);

    REQUIRE(wasCalled);
    REQUIRE(receivedValue == 42);
}

TEST_CASE("Events.EventBus.Emit_WithNoSubscribersDoesNothing", "[EventBus]")
{
    EventBus bus;

    TestEventA event{ 1 };
    REQUIRE_NOTHROW(bus.Emit(event)); // no subscribers — should be a safe no-op
}

TEST_CASE("Events.EventBus.MultipleSubscribers_AllReceiveTheEvent", "[EventBus]")
{
    EventBus bus;
    int callCount = 0;

    bus.Subscribe<TestEventA>([&](TestEventA&) { callCount++; });
    bus.Subscribe<TestEventA>([&](TestEventA&) { callCount++; });
    bus.Subscribe<TestEventA>([&](TestEventA&) { callCount++; });

    TestEventA event{ 0 };
    bus.Emit(event);

    REQUIRE(callCount == 3);
}

TEST_CASE("Events.EventBus.DifferentEventTypesDoNotCrossFire", "[EventBus]")
{
    EventBus bus;
    bool aCalled = false;
    bool bCalled = false;

    bus.Subscribe<TestEventA>([&](TestEventA&) { aCalled = true; });
    bus.Subscribe<TestEventB>([&](TestEventB&) { bCalled = true; });

    TestEventA eventA{ 1 };
    bus.Emit(eventA);

    REQUIRE(aCalled);
    REQUIRE_FALSE(bCalled); // subscribing to A must not fire on B
}

TEST_CASE("Events.EventBus.Unsubscribe_StopsFurtherDelivery", "[EventBus]")
{
    EventBus bus;
    int callCount = 0;

    SubscriptionId id = bus.Subscribe<TestEventA>([&](TestEventA&) { callCount++; });

    TestEventA event{ 0 };
    bus.Emit(event); // first call, should count

    bus.Unsubscribe(id);
    bus.Emit(event); // second call, should NOT count

    REQUIRE(callCount == 1);
}

TEST_CASE("Events.EventBus.Unsubscribe_OnlyRemovesTheTargetedSubscription", "[EventBus]")
{
    EventBus bus;
    int firstCallCount = 0;
    int secondCallCount = 0;

    SubscriptionId firstId = bus.Subscribe<TestEventA>([&](TestEventA&) { firstCallCount++; });
    bus.Subscribe<TestEventA>([&](TestEventA&) { secondCallCount++; });

    bus.Unsubscribe(firstId);

    TestEventA event{ 0 };
    bus.Emit(event);

    REQUIRE(firstCallCount == 0);
    REQUIRE(secondCallCount == 1);
}

TEST_CASE("Events.EventBus.Unsubscribe_UnknownIdIsSafeNoOp", "[EventBus]")
{
    EventBus bus;

    REQUIRE_NOTHROW(bus.Unsubscribe(999)); // never subscribed — should not throw or crash
}

TEST_CASE("Events.EventBus.Unsubscribe_CalledTwiceIsSafe", "[EventBus]")
{
    EventBus bus;
    SubscriptionId id = bus.Subscribe<TestEventA>([](TestEventA&) {});

    bus.Unsubscribe(id);
    REQUIRE_NOTHROW(bus.Unsubscribe(id)); // second call on an already-removed id
}

TEST_CASE("Events.EventBus.SubscriptionIdsAreUnique", "[EventBus]")
{
    EventBus bus;

    SubscriptionId id1 = bus.Subscribe<TestEventA>([](TestEventA&) {});
    SubscriptionId id2 = bus.Subscribe<TestEventA>([](TestEventA&) {});
    SubscriptionId id3 = bus.Subscribe<TestEventB>([](TestEventB&) {});

    REQUIRE(id1 != id2);
    REQUIRE(id2 != id3);
    REQUIRE(id1 != id3);
}

TEST_CASE("Events.EventBus.MultipleEmitsAllDeliverToActiveSubscriber", "[EventBus]")
{
    EventBus bus;
    int callCount = 0;

    bus.Subscribe<TestEventA>([&](TestEventA&) { callCount++; });

    TestEventA event{ 0 };
    bus.Emit(event);
    bus.Emit(event);
    bus.Emit(event);

    REQUIRE(callCount == 3);
}