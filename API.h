
//chick API header file
//// File name: API.h
//// Author: Quan Guo
//   GTID:903241776
//   Username: qguo48


#ifndef API_H_
#define API_H_

/*  design a data structure for event
    time stamp describes the interval between current time and the time when this event will happen
    AppData (pointer to another structure) describes what kind of event it is
    Next, pointer to Event structure, describes next event that is most close to this one on time line
*/

typedef struct Event {

    double timestamp;
    void*  AppData;
    struct Event* Next;

} Event;

/*  design a data structure for event data
    structure contains only one element representing what kind of event it is
*/
typedef struct EventData {

    int EventType;

} EventData;

//function to generate a random floating number between [0,1)
double urand(void);

//function generate an exponential value according to homework requirement
double randexp(double U);


//function generate a value between Bottom value and Top value uniformly
double uniform(double bottom, double top);

// Function to execute simulation up to a specified time (EndTime)
void RunSim (double EndTime);

// Schedule an event with time stamp ts, event parameters *data
void Schedule (double ts, void *data);

// This function returns the current simulation time
double CurrentTime (void);

// Function defined in the simulation application called by the simulation engine
// Event handler function: called to process an event
void EventHandler (void *data);

//initialize the calendar queue
void localInit(int qbase, int nbuck, double bwidth, double startprio);

void presentTime(int t);

#endif
