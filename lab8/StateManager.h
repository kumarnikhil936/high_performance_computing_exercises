#pragma once

#include "State.h"
#include <unordered_set>





class Playfield{
public:
    enum GoalType{Left = 0, Top = 1, Right = 2, Bot = 3};
    
    Playfield(int x,int y,GoalType g)
    :sizeX(x),sizeY(y),goal((int)g)
    {}
    
    
    int sizeX;
    int sizeY;
    
    int goal; //0 left, 1 top, 2 right 3 bot
};


/*
 Task 3
 ------
 StateManager is responsible for state that is shared across all tasks, yet lacks any precautions against dataraces.
 add a Mutex (omp_lock_t from <omp.h> or std::Mutex from <mutex>) and use it where necessary to make the program threadsafe.
 
 
 Bonus Task (not graded)
 ------
 C++17 offers a reader/writer lock called std::shared_mutex (in <shared_mutex>), which allows multiple concurrent reads
 if locked with the proper function. Attempt to improve the efficiency of StateManager by locking as a reader whenever possible
 and only locking as a writer when a write is necessary.
 
 */

class StateManager{
public:
    StateManager(Playfield p)
    :playfield(p),best_solution_size(10000),best_solution()//,max_claim(0)
    {
        omp_init_lock(&lck1);
        omp_init_lock(&lck2);
        
    }
    
    ~StateManager() {
        omp_destroy_lock(&lck1);
        omp_destroy_lock(&lck2);
    }
    
    
    
    //Size of the best solution found so far
    int bestSolutionSize() const{ return best_solution_size;};
    
    //Enter a new solution. It replaces the previous one if its faster to reach.
    void enterSolution(const State& res);
    
    /*
     Attempts to claim a configuration for this task. This works if the configuration does not exist
     or exists, but takes longer than the one we are inserting.
     Returns whether we got the claim or it is already taken.
     */
    bool claim(const State& val);
    
    void printBestSolution();
    
    omp_lock_t lck1, lck2;
    
    Playfield playfield;
private:
    int best_solution_size;
    State best_solution;
    
    
    std::unordered_set<State,StateHash,StateEqual> state_set;
    
};
