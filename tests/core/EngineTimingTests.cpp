#include "core/Engine.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("Core.TimeTracker.GetDeltaTime_ComputesElapsedSeconds", "[TimeTracker]")
{
    // frequency = 1000 ticks/sec, so 20 ticks = 0.02 sec — comfortably under MAX_DELTA_TIME
    TimeTracker tracker(0, 1000);

    double deltaTime = tracker.GetDeltaTime(20);

    REQUIRE(deltaTime == Catch::Approx(0.02));
}

TEST_CASE("Core.TimeTracker.GetDeltaTime_UpdatesInternalTimeEachCall", "[TimeTracker]")
{
    TimeTracker tracker(0, 1000);

    double first = tracker.GetDeltaTime(20);   // 0 -> 20: 0.02s
    double second = tracker.GetDeltaTime(50);  // 20 -> 50: 0.03s

    REQUIRE(first == Catch::Approx(0.02));
    REQUIRE(second == Catch::Approx(0.03));
}

TEST_CASE("Core.TimeTracker.GetDeltaTime_ClampsToMaxDeltaTime", "[TimeTracker]")
{
    // A huge gap (e.g. debugger pause) should clamp rather than spike deltaTime
    TimeTracker tracker(0, 1000);

    double deltaTime = tracker.GetDeltaTime(10000); // would be 10.0s unclamped

    REQUIRE(deltaTime == Catch::Approx(0.05)); // MAX_DELTA_TIME
}

TEST_CASE("Core.FixedTimer.Update_ReturnsFalseBelowThreshold", "[FixedTimer]")
{
    FixedTimer timer;

    timer.Increment(0.01); // well under 1/60s

    REQUIRE_FALSE(timer.Update());
}

TEST_CASE("Core.FixedTimer.Update_ReturnsTrueOnceThresholdReached", "[FixedTimer]")
{
    FixedTimer timer;

    timer.Increment(1.0 / 60.0);

    REQUIRE(timer.Update());
}

TEST_CASE("Core.FixedTimer.Update_FiresMultipleTimesForLargeAccumulation", "[FixedTimer]")
{
    FixedTimer timer;

    timer.Increment(3.0 / 60.0); // three timesteps worth

    int updateCount = 0;
    while (timer.Update())
        updateCount++;

    REQUIRE(updateCount == 3);
}

TEST_CASE("Core.FixedTimer.Update_ConsumesAccumulatorPerCall", "[FixedTimer]")
{
    FixedTimer timer;
    timer.Increment(1.0 / 60.0);

    REQUIRE(timer.Update());   // consumes the one available timestep
    REQUIRE_FALSE(timer.Update()); // nothing left
}

TEST_CASE("Core.FPSMeter.Update_ReturnsFalseBeforeOneSecond", "[FPSMeter]")
{
    FPSMeter meter(0, 1000); // 1000 ticks/sec

    bool updated = meter.Update(500); // only 0.5s elapsed

    REQUIRE_FALSE(updated);
}

TEST_CASE("Core.FPSMeter.Update_ReturnsTrueAfterOneSecond", "[FPSMeter]")
{
    FPSMeter meter(0, 1000);

    bool updated = meter.Update(1000); // exactly 1.0s elapsed

    REQUIRE(updated);
}

TEST_CASE("Core.FPSMeter.Update_ComputesCorrectFPS", "[FPSMeter]")
{
    FPSMeter meter(0, 1000);

    for (int i = 0; i < 59; ++i)
        meter.Update(i); // 59 frames primed, none trigger (well under 1s)

    bool updated = meter.Update(1000); // 60th frame, crosses the 1s mark

    REQUIRE(updated);
    REQUIRE(meter.GetFPS() == 60);
}

TEST_CASE("Core.FPSMeter.Update_ResetsFrameCountAfterReporting", "[FPSMeter]")
{
    FPSMeter meter(0, 1000);

    meter.Update(1000); // first second: 1 frame counted, triggers report
    bool updatedAgain = meter.Update(1500); // only 0.5s since last report

    REQUIRE_FALSE(updatedAgain); // shouldn't re-trigger until another full second
}