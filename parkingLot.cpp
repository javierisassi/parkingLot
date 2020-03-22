#include <iostream>
#include <vector>
#include <algorithm>

int const MAX_LOT_SIZE = 50;

//TODO: make the vector private and only expose the needed functions
template<typename T>
class Cars : public std::vector<T>
{
    int _maxSize;
    public:
	void setMaxSize(int maxSize) {_maxSize = maxSize;}
	void push_back(const T& element)
	{
	    if (std::vector<T>::size() < _maxSize)
	       std::vector<T>::push_back(element);
	    else
	    {
	       std::cout << "PARKING LOT FULL\n" << std::endl;
	    }
	}
};

class Car{
    public:
        enum eCarType{ eStandard, eElectric, ehandycap, eCompact };
        Car( eCarType carType ){
            _carType = carType;
        }
        eCarType getCarType() const { return _carType; };
        void setCarType (Car::eCarType carType) { _carType = carType; };
        inline bool operator==(const Car& lhs){ 
	    return _carType == lhs.getCarType();}
    private:
        eCarType _carType; 
};

class ParkingLot{
    public:
        ParkingLot( int maxLotSize = MAX_LOT_SIZE ) {
            lot.setMaxSize(maxLotSize);
        }

        void enterStandard() {
            Car tCar(Car::eStandard);
            lot.push_back(tCar);
	}
        void enterElectric() {
            Car tCar(Car::eElectric);
            lot.push_back(tCar);
	}
        void enterCompact() {
            Car tCar(Car::eCompact);
            lot.push_back(tCar);
	}
        void enterhandycapped() {
            Car tCar(Car::ehandycap);
            lot.push_back(tCar);
	}

        void exitStandard() {
            //TODO: just use the type not the whole car
            Car car(Car::eStandard); //Do I have to make a car to look for it?
            discard(car);
        }
        void exitElectric() {
            Car car(Car::eElectric);
            discard(car);
        }
        void exitCompact() {
            Car car(Car::eCompact);
            discard(car);
        }
        void exithandycapped() {
            Car car(Car::ehandycap); 
            discard(car);
        }
        void discard(Car &car) {
            auto it = std::find(lot.begin(), lot.end(), car);
            if ( it != lot.end() )
                lot.erase(it);
        }
        struct Status { int total; int stand; int elect; int comp; 
            int handy; };
        Status getStatus(){
            Status status = {0};
            status.total = lot.size();
            //here iterate through vector and get totals.
            for ( std::vector<Car>::iterator it = lot.begin(); 
                    it != lot.end(); it++) {
                if (it->getCarType() == Car::eStandard )status.stand++;		
                if (it->getCarType() == Car::eElectric )status.elect++;		
                if (it->getCarType() == Car::ehandycap )status.handy++;		
                if (it->getCarType() == Car::eCompact )status.comp++;		
            }
            return status;
        }

        //free the vector
        ~ParkingLot(){ lot.clear(); };
    private:
        Cars <Car>lot;
};

// TEST HARNESS. Uncomment below to test ParkingLot
int main ( void ) {
    
    // TEST 1. Create a parking lot with 50 cars. Check is empty
    {
	ParkingLot pklot( 50 );
	ParkingLot::Status status = pklot.getStatus();
	if (status.total == 0 ) std::cout << "TEST 1 PASS " << std::endl;
	else std::cout << "TEST 1 FAIL " << status.total << std::endl;
    } // Notice. You exit scope, your parking lot should call destructor

        
    // TEST 2. Park 50 cars standard.
    {
	ParkingLot pklot( 100 );
        for ( int i = 0; i < 50; ++i)
            pklot.enterStandard();
	ParkingLot::Status status = pklot.getStatus();
	if ( status.total == 50 ) std::cout << "TEST 2 PASS " << std::endl;
	else std::cout << "TEST 2 FAIL " << status.total << std::endl;
    }

    // TEST 3. Park 50 cars standard 12 exit. Total should be 38
    {
	ParkingLot pklot( 100 );
        for ( int i = 0; i < 50; ++i)
            pklot.enterStandard();
        for ( int j = 0; j < 12; ++j)
            pklot.exitStandard();
	ParkingLot::Status status = pklot.getStatus();
	if ( status.total == 38 ) std::cout << "TEST 3 PASS " << std::endl;
	else std::cout << "TEST 3 FAIL " << status.total << std::endl;
    }

    // TEST 4. Lets turn drivers away, try to park 121 standard cars
    // Total should be 100
    {
	ParkingLot pklot( 100 );
        for ( int i = 0; i < 121; ++i)
            pklot.enterStandard();
	ParkingLot::Status status = pklot.getStatus();
	if ( status.total == 100 ) std::cout << "TEST 4 PASS " << std::endl;
	else std::cout << "TEST 4 FAIL " << status.total << std::endl;
    }

    // TEST 5. Lets empty the lot, try to park 90 standard cars
    // and then removem. total should be 0
    {
	ParkingLot pklot( 100 );
        for ( int i = 0; i < 90; ++i)
            pklot.enterStandard();
        for ( int i = 0; i < 90; ++i)
            pklot.exitStandard();
	ParkingLot::Status status = pklot.getStatus();
	if ( status.total == 0 ) std::cout << "TEST 5 PASS " << std::endl;
	else std::cout << "TEST 5 FAIL " << status.total << std::endl;
    }

    // TEST 6. Mix and match, lets park all kind of cars
    // 9 standard 13 electric 7 handycapped 17 compact
    {
	ParkingLot pklot( 100 );
        for ( int i = 0; i < 9; ++i)
            pklot.enterStandard();
        for ( int i = 0; i < 13; ++i)
            pklot.enterElectric();
        for ( int i = 0; i < 7; ++i)
            pklot.enterhandycapped();
        for ( int i = 0; i < 17; ++i)
            pklot.enterCompact();
	ParkingLot::Status status = pklot.getStatus();
	if ( status.stand == 9 &&
             status.elect == 13 &&
             status.handy == 7 &&
             status.comp == 17 ) 
           std::cout << "TEST 6 PASS " << std::endl;
	else std::cout << "TEST 6 FAIL " << status.total << std::endl;
    }

    // TEST 7. LAST test
    // 9 standard 13 electric 7 handycapped 17 compact exit 2 handy 4 compact
    {
	ParkingLot pklot( 100 );
        for ( int i = 0; i < 9; ++i)
            pklot.enterStandard();
        for ( int i = 0; i < 13; ++i)
            pklot.enterElectric();
        for ( int i = 0; i < 7; ++i)
            pklot.enterhandycapped();
        for ( int i = 0; i < 17; ++i)
            pklot.enterCompact();
        for ( int i = 0; i < 2; ++i)
            pklot.exithandycapped();
        for ( int i = 0; i < 4; ++i)
            pklot.exitCompact();
	ParkingLot::Status status = pklot.getStatus();
	if ( status.stand == 9 &&
             status.elect == 13 &&
             status.handy == 5 &&
             status.comp == 13 &&
             status.total == 40) 
           std::cout << "TEST 7 PASS " << std::endl;
	else std::cout << "TEST 7 FAIL " << status.total << std::endl;
    }
    return 0;
}
