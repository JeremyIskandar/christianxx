#include "analysis.h"
#include <bits/stdc++.h>

using std::vector;
using std::pair;

//! @todo
//! TASK 1 - We need to store cars in the Analysis class, how to do this?
Analysis::Analysis(std::vector<CarInterface*> cars) :
    cars_(cars),raceDisplay_(nullptr)
{

}

// std::vector<CarInterface*> cars_.push_back(cars); ??

Analysis::Analysis(std::vector<CarInterface*> cars,std::shared_ptr<DisplayRace> raceDisplay) :
    cars_(cars),raceDisplay_(raceDisplay)
{

}

//! @todo
//! TASK 1 - Refer to README.md and the Header file for full description
std::vector<unsigned int> Analysis::sortByOdometry(){

    std::vector<unsigned int> order(cars_.size(),0);//Creating a vector, same size as cars with all zeros
    std::vector<pair<int, int> >vp; //vector store odometry + id

    for (int i = 0;i<cars_.size();i++){     //loop to store odom + id
        vp.push_back(std::make_pair(cars_[i]->getOdometry(),cars_[i]->getID()));
    }

    sort(vp.begin(), vp.end());     //sort odometry
    // vp[i].second  ->  will convey the index order starting at i=0

    for (int i = 0;i<cars_.size();i++){     // push index into order vec
        order[i] = vp[i].second;
    }

    return order;
}

    // std::vector<unsigned int> index_test(cars_.size(),0);

    // std::vector<unsigned int> ID_test(cars_.size(), 0);

    // std::vector<unsigned int> odo_test(cars_.size(), 0);
    
    // for (int i = 0;i<cars_.size();i++){     //loop to store odom + index
    //     vp.push_back(std::make_pair(cars_[i]->getOdometry(),i));
    //     ID_test[i] = cars_[i]->getID();
    //     odo_test[i] = cars_[i]->getOdometry();
    // }

    // sort(vp.begin(), vp.end());     //sort odometry
    // // vp[i].second  ->  will convey the index order starting at i=0

    // for (int i = 0;i<cars_.size();i++){     // push index into order vec
    //     index_test[i] = vp[i].second;
    //     odo_test[i] = cars_[vp[i].second]->getOdometry();
    //     ID_test[i] = cars_[vp[i].second]->getID();
    //     order[i] = (cars_[vp[i].second]->getID());
    // }


//! @todo
//! TASK 2 - Refer to README.md and the Header file for full description
std::vector<unsigned int> Analysis::dragRace(double distance){

    std::vector<unsigned int> order(cars_.size(),0);//Creating a vector, same size as cars with all zeros
    std::vector<unsigned int> initial;
    std::vector<unsigned int> vpp;
    std::vector<bool> booly(cars_.size(), false);   // vector bool to check if we have 

    auto size = cars_.size();
    int done = 0;
    bool check_bool = false;     // to check vec booly if all condititions true

    for (auto it:cars_){
        initial.push_back(it->getOdometry());  //initial odometry values
    }

    while(!check_bool){
        std::vector<std::pair<int,int>>vp;  //initialize vector pair vp       
        for(int i=0;i<size;i++){
            cars_[i]->accelerate();     //accelerate each car
            unsigned int subtracted = cars_[i]->getOdometry() - initial[i];     //get distance travelled subtracting init from curr
            if (subtracted >= distance & !booly[i]){    // check if substracted distance is greater than passed dist + if bool 0 has not done
                booly[i] = 1;
                check_bool = std::all_of(booly.begin(), booly.end(), [](bool b) { return b; });   //checks if all bools have passed
                vp.push_back(std::make_pair(subtracted, cars_[i]->getID()));
            }
            if(raceDisplay_!=nullptr){
                raceDisplay_->updateDisplay();
            }
        }
        std::sort(vp.begin(), vp.end(), std::greater<std::pair<int,int>>());
        for(auto pair: vp){         // sort from least to greatest distance assuming they would pass 100m if distance is greater
            vpp.push_back(pair.second);
        }
    }
    for (int i = 0;i<size;i++){     // push index into order vec
        order[i] = vpp[i];
    }
    return order;
}

// std::vector<unsigned int> Analysis::dragRace(double distance){

    // std::vector<unsigned int> order(cars_.size(),0);
    // std::vector<pair<int, int> >vp;
    // std::vector<pair<int, int> >vpp;
    // std::vector<bool> check_index(cars_.size(),0);

    // bool finished = false;    // check if all values 1 in vec

    // for (int i = 0;i<cars_.size();i++){     //loop to store odom + id
    //     vp.push_back(std::make_pair(cars_[i]->getOdometry(),cars_[i]->getID()));   //fill vpair with odo.first and id.2nd
    // }

    // while (!finished){
    //     for(auto car : cars_){
    //         //unsigned int tester = car->getOdometry();
    //         car->accelerate();
    //         auto index = std::distance(cars_.begin(), std::find(cars_.begin(), cars_.end(), car));
    //         //int car_odometry = car->getOdometry();
    //         //int car_old = vp[index].first;
    //         //int substraction = car_odometry - car_old;
    //         if(car->getOdometry() - vp[index].first >= distance & !check_index[index]){
    //             check_index[index] = true;
    //             vpp.push_back(std::make_pair(car->getOdometry()-vp[index].first,car->getID()));
    //             finished = std::all_of(check_index.begin(), check_index.end(), [](bool b) { return b; });
    //         }
    //         else{
    //             {} //do nothing
    //         }
    //     }
    // }
    // sort(vpp.begin(), vpp.end()); 
    // for (auto it:vpp){
    //     order[std::distance(vpp.begin(), std::find(vpp.begin(), vpp.end(), it))]= it.second;
    // }
    
    // return order;
//     }
    

//! @todo
//! TASK 3 - Refer to README.md and the Header file for full description
void Analysis::stopAllCars(){
    const double epsilon = 1e-9;
    bool keep = false;
    std::vector<bool> cars_stopped(cars_.size(),false);
    
    while(!keep){
        for(auto car : cars_){
            car->decelerate();
            auto index = std::distance(cars_.begin(), std::find(cars_.begin(), cars_.end(), car));
            if(car->getCurrentSpeed()<epsilon){
                cars_stopped[index] = true;
                keep = std::all_of(cars_stopped.begin(), cars_stopped.end(), [](bool b) { return b;});
            }
            else{
                {}  //do nothing
            }
        }
    }
    
}

//! @todo
//! TASK 4 - Refer to README.md and the Header file for full description
std::vector<unsigned int> Analysis::zeroTopZeroRace(){
    std::vector<unsigned int> order(cars_.size(),0);//Creating a vector, same size as cars with all zeros
    std::vector<bool>car_has_accel(cars_.size(),false);
    std::vector<bool>car_has_decel(cars_.size(),false);
    std::vector<int>idVec;

    bool check_bool = false;
    const double epsilon = 1e-9;
    int size = cars_.size();

    while(!std::all_of(car_has_decel.begin(), car_has_decel.end(), [](bool b) { return b; })){
        std::vector<std::pair<int,int>>vp;  //initialize vector pair vp       
        for(int i=0;i<size;i++){
            if(!car_has_accel[i]){
                cars_[i]->accelerate();     //accelerate each car
                if(raceDisplay_!=nullptr){
                    raceDisplay_->updateDisplay();
                }
                std::cout<<"Current speed for "<<cars_[i]->getModel()<<" is: "<<cars_[i]->getCurrentSpeed()<<" with top speed: "<<cars_[i]->getTopSpeed()<<" at index "<<i<<std::endl;
            }
            if(cars_[i]->getCurrentSpeed() >= cars_[i]->getTopSpeed() | car_has_accel[i]){    // check if substracted distance is greater than passed dist + if bool 0 has not done
                if(car_has_accel[i]){
                    cars_[i]->decelerate();
                    if(raceDisplay_!=nullptr){
                        raceDisplay_->updateDisplay();
                    }
                    std::cout<<"Current speed for "<<cars_[i]->getModel()<<" is: "<<cars_[i]->getCurrentSpeed()<<" with top speed: "<<cars_[i]->getTopSpeed()<<" at index "<<i<<std::endl;
                    if(cars_[i]->getCurrentSpeed() <= epsilon | car_has_decel[i]){
                        if(!car_has_decel[i]){
                            idVec.push_back(cars_[i]->getID());
                            car_has_decel[i] = true;
                        }
                        else{
                            {}    //do nothing
                        }
                        
                    }
                }
                else{
                    car_has_accel[i] = true;
                }
            }

        }
    }
    for (int i = 0;i<size;i++){     // push index into order vec
        order[i] = idVec[i];
    }

    return order;
}

/*
    std::vector<unsigned int> order(cars_.size(),0);//Creating a vector, same size as cars with all zeros
    std::vector<double>topSpeeds(cars_.size(),0);
    std::vector<double>boolAcc(cars_.size(),false);
    std::vector<double>boolDcc(cars_.size(),false);
    std::vector<int>idVec;

    bool some_bool = false;
    const double epsilon = 1e-3;

    for(int i=0;i<cars_.size();i++){
        topSpeeds[i] = cars_[i]->getTopSpeed();
    }

    while(!std::all_of(boolDcc.begin(), boolDcc.end(), [](bool b) { return b; })){
        for(auto car : cars_){
            auto index = std::distance(cars_.begin(), std::find(cars_.begin(), cars_.end(), car));
            int currspeed = car->getCurrentSpeed();
            int topspeed = car->getTopSpeed();
            std::cout<<"Current speed for "<<car->getModel()<<" is: "<<car->getCurrentSpeed()<<" with top speed: "<<car->getTopSpeed()<<" at index "<<index<<std::endl;
            if (currspeed >= topspeed){
                if(!boolAcc[index]){
                    boolAcc[index] = true;
                    std::cout<<boolAcc[index]<<std::endl;
                }
                car->decelerate();
                std::this_thread::sleep_for(std::chrono::seconds(1));
                currspeed = car->getCurrentSpeed();
                // if(raceDisplay_!=nullptr){
                //     raceDisplay_->updateDisplay();
                // }
                if(currspeed<=epsilon){
                    std::cout<<"Current speed for "<<car->getModel()<<" is: "<<car->getCurrentSpeed()<<std::endl;
                    idVec.push_back(car->getID());
                    if(!boolDcc[index]){
                        boolDcc[index] = true;
                        std::cout<<boolDcc[index]<<std::endl;
                    }
                }
                else{
                    {}  //do nothing
                }
            }
            else if (!boolAcc[index]){
                car->accelerate();
                std::this_thread::sleep_for(std::chrono::seconds(1));
                // if(raceDisplay_!=nullptr){
                //     raceDisplay_->updateDisplay();
                // }
            }
        }
    }
    for (int i = 0;i<cars_.size();i++){     
            order[i] = idVec[i];
        }

    return order;
*/


// Demo code
void Analysis::demoRace(){


    //This is an example of how to draw 3 cars moving
    // accelerate 300 times
    unsigned int count=0;

    while(count < 300){
        for(auto car : cars_){
          car->accelerate();
        }
        if(raceDisplay_!=nullptr){
            raceDisplay_->updateDisplay();
        }
        count++;
    }

    // decelerate 600 times
    count =0;
    while(count < 600){
        for(auto car : cars_){
          car->decelerate();
        }
        if(raceDisplay_!=nullptr){
            raceDisplay_->updateDisplay();
        }
        count++;
    }

}
