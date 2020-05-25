#include <iostream>
#include <random>
#include "TrafficLight.h"



/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
    std::unique_lock<std::mutex> uLock(_mtx);

    // puts thread to wait state and only resumes when new data is available
    _cond.wait(uLock, [this] { return !_queue.empty(); });

    // pull the message from the queue using move semantics
    T msg = std::move(_queue.back());
    _queue.pop_back();

    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.

    std::lock_guard<std::mutex> lck(_mtx);
    std::cout << "  Message has been send to the queue." << "\n";

    // add the message to the  queue and  then send notification 
    _queue.push_back(std::move(msg));
    _cond.notify_one();   // it only wakes up a single waiting thread
}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
    while (true)
    {
        TrafficLightPhase v = _messages.receive();
        std::lock_guard<std::mutex> lck(_mutex);
        if (v == TrafficLightPhase::green)   // once it receives the green 
        {
            std::cout << "  The Light in thread # " << std::this_thread::get_id() << " has turn to green." << "\n";
            break;
        }
    }


}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 

    std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    lastUpdate = std::chrono::system_clock::now();
    //std::random_device is a uniformly-distributed integer random 
    // number generator that produces non-deterministic random numbers.
    std::random_device rd;
    // Mersenne Twister 19937 generator --  A Mersenne Twister pseudo-random generator of 32-bit number
    std::mt19937 eng(rd());
    // Produces random floating-point values i, uniformly distributed on // the interval [a, b), that is, distributed according to the 
    // probability density function
    std::uniform_int_distribution<int> dist(4000, 6000); 

    double cycleDuration = dist(eng); // duration of a single simulation cycle in ms

    
     while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        
        if (timeSinceLastUpdate >= cycleDuration)
        {
            // take a duration by 4-6 seconds.
            std::this_thread::sleep_for(std::chrono::milliseconds(dist(eng)));
            if (_currentPhase == TrafficLightPhase::red)
            {
                _messages.send(std::move(TrafficLightPhase::green));
                // std::lock_guard<std::mutex> lck(_mutex);
                _currentPhase = TrafficLightPhase::green;
            }
            else
            {
                _messages.send(std::move(TrafficLightPhase::red));
                // std::lock_guard<std::mutex> lck(_mutex);
                _currentPhase = TrafficLightPhase::red;
            }
            
            lastUpdate = std::chrono::system_clock::now();
        }
    }
    
}

