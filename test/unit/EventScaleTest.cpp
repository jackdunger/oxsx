#include <catch.hpp>
#include <Event.h>
#include <EventScale.h>

TEST_CASE("Simple scale on variable number 3"){
    EventScale scaler("scaler");
    scaler.SetObservables(ObsSet(3));
    scaler.SetScale(2);

    // make fake event
    std::vector<double> fakeObservations;
    fakeObservations.push_back(0);
    fakeObservations.push_back(1);
    fakeObservations.push_back(2);
    fakeObservations.push_back(3);

    Event inData(fakeObservations);
    
    // apply the shift
    Event outData = scaler(inData);
    std::vector<double> modifiedObs = outData.GetData();

    // Check the values
    std::vector<double> correctVals = fakeObservations;
    correctVals[3] *= 2;

    REQUIRE(modifiedObs == correctVals);
}
