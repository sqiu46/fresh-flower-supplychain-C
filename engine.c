
//// File name: Chick_engine.c
//// Author: Quan Guo
// GTID:903241776
//Username: qguo48

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include "API.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//
// General Purpose Discrete Event Simulation Engine
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// Simulation Engine Data Structures
/////////////////////////////////////////////////////////////////////////////////////////////
// Data structure for an event; this is independent of the application domain


// Simulation clock variable
double Now = 0.0;

// Future Event List
// Use an event structure as the header for the future event list (priority queue)
struct Event FEL ={-1.0, NULL, NULL};

/////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes for functions used within the Simulation Engine
/////////////////////////////////////////////////////////////////////////////////////////////

// Function to remove smallest timestamped event
Event *Remove (void);

/////////////////////////////////////////////////////////////////////////////////////////////
// Simulation Engine Functions Internal to this module
/////////////////////////////////////////////////////////////////////////////////////////////

// Remove smallest timestamped event from FEL, return pointer to this event
// return NULL if FEL is empty
Event *Remove (void)
{
    struct Event *e;

    if (FEL.Next==NULL) return (NULL);
    e = FEL.Next;		// remove first event in list
    FEL.Next = e->Next;
    return (e);
}


/////////////////////////////////////////////////////////////////////////////////////////////
// Simulation Engine functions visible to simulation application
/////////////////////////////////////////////////////////////////////////////////////////////

// Return current simulation time
double CurrentTime (void)
{
	return Now;
}

// Schedule new event in FEL
// queue is implemented as a timestamp ordered linear list
void Schedule (double ts, void *data)
{
	struct Event *e, *p, *q;

	// create event data structure and fill it in
	if ((e = malloc (sizeof (struct Event))) == NULL) exit(1);
	e->timestamp = ts;
	e->AppData = data;

	// insert into priority queue
	// p is lead pointer, q is trailer
	for (q=&FEL, p=FEL.Next; p!=NULL; p=p->Next, q=q->Next) {
		if (p->timestamp > e->timestamp) break;
		}
	// insert after q (before p)
	e->Next = q->Next;
	q->Next = e;
}

// Function to execute simulation up to a specified time (EndTime)
void RunSim (double EndTime)
{
	struct Event *e;

	// Main scheduler loop
	while ((e=Remove()) != NULL) {
		Now = e->timestamp;
		printf("\n");
		int t = (int) Now;
		presentTime(t);
        if (Now > EndTime) break;
		EventHandler(e->AppData);
		free (e);	// it is up to the event handler to free memory for parameters

	}
}

//function to generate a random floating number between [0,1)
double urand(void)
{
    double U_R;
    double R = (double) rand();
    U_R = R/(RAND_MAX + 1);
    if (U_R < 0) {
        U_R = - U_R;
    }
    return U_R;
}

//function generate a random exponential value according to homework requirement
double randexp(double U)
{
    double t;
    double a = log(1.0-urand());
    t = -U*(log(1.0-urand()));
    if(a >= 0) {printf("wrong return value in randexp\n"); exit(1);}
    return t;
}

double uniform(double bottom, double top)
{
    double p;
    p = (double)rand()/RAND_MAX;
    double u;
    u = p * (top - bottom) + bottom;
    if (u<bottom || u>top) {
        printf("Wrong Uniform Value: %lf is out of [%lf,%lf]\n",u,bottom,top);
        exit(1);
    }
    return u;
}

