#include <catch.hpp>
#include <Event.h>
#include <EventShift.h>

TEST_CASE("Simple shift on variable number 2"){
    EventShift shifter("shifter");
    shifter.SetObservables(ObsSet(2));
    shifter.SetShift(1);

    // make fake event
    std::vector<double> fakeObservations;
    fakeObservations.push_back(0);
    fakeObservations.push_back(1);
    fakeObservations.push_back(2);
    fakeObservations.push_back(3);

    Event inData(fakeObservations);
    
    // apply the shift
    Event outData = shifter(inData);
    std::vector<double> modifiedObs = outData.GetData();

    // Check the values
    std::vector<double> correctVals = fakeObservations;
    correctVals[2] += 1;

    REQUIRE(modifiedObs == correctVals);
}
