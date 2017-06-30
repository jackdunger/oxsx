#include <catch.hpp>
#include <EventSystematicManager.h>
#include <EventScale.h>
#include <EventShift.h>
#include <Event.h>
#include <iostream>

TEST_CASE("3 observable event, shift and scale"){
    // Create the systematics
    EventScale scaler("scaler");
    scaler.SetScale(3);

    EventShift shifter("shifter");
    shifter.SetShift(1);

    // Create a fake event
    std::vector<double> data(3, 1);
    Event event(data);

    SECTION("Acting of different observables"){
        scaler.SetObservables(ObsSet(0));
        shifter.SetObservables(ObsSet(1));

        EventSystematicManager sysMan;
        sysMan.Add(&scaler);
        sysMan.Add(&shifter);

        // transform
        Event modified = sysMan.ApplySystematics(event);

        // Test
        REQUIRE(modified.GetData().size() == 3);
        REQUIRE(modified.GetData().at(0) == 3);
        REQUIRE(modified.GetData().at(1) == 2);
        REQUIRE(modified.GetData().at(2) == 1);
    }

    SECTION("Acting on same observables"){
        // Act in the order added to sysMan
        scaler.SetObservables(ObsSet(0));
        shifter.SetObservables(ObsSet(0));

        EventSystematicManager sysMan;
        sysMan.Add(&scaler);
        sysMan.Add(&shifter);

        // transform
        Event modified = sysMan.ApplySystematics(event);
        // Test
        REQUIRE(modified.GetData().size() == 3);
        REQUIRE(modified.GetData().at(0) == (1 * 3 + 1) );
        REQUIRE(modified.GetData().at(1) == 1);
        REQUIRE(modified.GetData().at(2) == 1);
    }


    SECTION("Acting on same observables - otherway around"){
        // Act in the order added to sysMan
        scaler.SetObservables(ObsSet(0));
        shifter.SetObservables(ObsSet(0));

        EventSystematicManager sysMan;
        sysMan.Add(&shifter);
        sysMan.Add(&scaler);

        // transform
        Event modified = sysMan.ApplySystematics(event);

        // Test
        REQUIRE(modified.GetData().at(0) == ((1 + 1) * 3) );
    }
}
