#include <stdio.h>
#include <time.h>
#include "API.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>


#define WORKER_PREPROCESS 1
#define DESIGNER_PREPROCESS 2
#define DESIGNER_BUNCH 3
#define FLOWER_DISPOSAL 4
#define WORKER_PREPROCESS 1
#define DESIGNER_PREPROCESS 2
#define DESIGNER_BUNCH 3
#define FLOWER_DISPOSAL 4
#define A_SHIP_TO_C 5
#define A_SHIP_TO_M 6
#define M_SHIP_TO_C 7
#define SHIP_BACK 8
#define ARRIVAL 9
#define CHECK_OUT 10
#define CALL_SUPPLEMENT 11
#define RETAIL   12


void worker_preprocess(EventData *a);
void designer_preprocess(EventData *a);
void designer_bunch(EventData *a);
void flower_disposal(EventData *a);

void A_Ship_to_C(EventData *a);
void A_Ship_to_M(EventData *a);
void M_Ship_to_C(EventData *a);
void Ship_Back(EventData *a);

void arrival(EventData *a);
void check_out(EventData *a);
void call_supplement(EventData *a);
void retail(EventData *a);

void CheckClock(double ts,double CloseT);
void BuyWhat();
int HowMany(int a);
void CheckStorage();
void MakeOrder();

int Worker = 10;
int Designer = 3;
int Process_Worker = 0;
int Process_Designer = 0;
int Bunch_Designer = 0;
double Worker_Salary = 30;
double Designer_Salary = 80;

int Flower = 0;
int Bunch = 0;
int A_Flower = 0;
int A_Bunch = 0;
int M_Flower = 0;
int M_Bunch = 0;
int C_Flower;
int C_Bunch;
int Spare_Flower = 0;
int Spare_Bunch = 0;

double Ideal_Income;
double Actual_Income;
double PF = 0.99;
double PB = 14.99;
double Cost_Disposal = 0.0;
double Cost_Ship_Back = 0.0;

int ASC_Flower = 0;
int ASC_Bunch = 0;
int ASM_Flower = 0;
int ASM_Bunch = 0;
int MSC_Flower = 0;
int MSC_Bunch = 0;
int First_Order_Flower = 300;
int First_Order_Bunch = 100;

int Sale_bunch = 0;
int Spare_bunch = 0;
double cost_disposal = 0.0;


///// Company A
double effi_worker;
double effi_designer;
double effi_bunch;
double* range_effi_worker;
double* range_effi_designer;
double* range_effi_bunch;
double A_CloseT = 3600.0*8;
double C_CloseT = 3600.0*18;

double WorkerProcessT;
double DesignerProcessT;
double DesignerBunchT;
bool OffProduction = false;
bool OffSale = false;
///// Company C
double A = 1.0; //how many customers arrive in one minute (actual rate following poission distribution)
bool buy_flower;
bool StorageRunOut = false;
bool HaveCalled = false;
int FlowerBuy = 0;
int BunchBuy = 0;
int FlowerSell;
int BunchSell;
int totalFlowerSell;
int totalBunchSell;
double Order_Flower;
double Order_Bunch;

int Day = 0;

void worker_preprocess(EventData *a){

    if(a->EventType != WORKER_PREPROCESS) {
        fprintf(stderr, "Incorrect Event Type: NOT WORKER_PREPROCESS\n");
        exit(1);
    } else {printf("worker preprocess\n");}

    //int FP = (int) (effi_worker * WorkerProcessT);
    Flower++;
    A_Flower++;

    Process_Worker--;
    Worker++;

    EventData* e = (EventData*)malloc(sizeof(EventData));
    if(e == NULL){fprintf(stderr, "invalid malloc\n"); exit(1);}
    e->EventType = WORKER_PREPROCESS;
    double ts;
    effi_worker = uniform(range_effi_worker[0],range_effi_worker[1]);
    WorkerProcessT = 1.0 / effi_worker;
	ts = CurrentTime() + WorkerProcessT*60;
	CheckClock(ts,A_CloseT);
	if(!OffProduction){
        Schedule(ts, e);
        Process_Worker++;
        Worker--;
	}
}


void designer_preprocess(EventData *a){

    if(a->EventType != DESIGNER_PREPROCESS) {
        fprintf(stderr, "Incorrect Event Type: NOT DESIGNER_PREPROCESS\n");
        exit(1);
    } else {printf("designer preprocess\n");}

    //int FP = (int) (effi_designer * DesignerProcessT);
    Flower++;
    A_Flower++;

    Process_Designer--;
    Designer++;

    EventData* e = (EventData*)malloc(sizeof(EventData));
    if(e == NULL){fprintf(stderr, "invalid malloc\n"); exit(1);}
    double ts;

    if (Flower >= 12) {
        e->EventType = DESIGNER_BUNCH;
        effi_bunch = uniform(range_effi_bunch[0],range_effi_bunch[1]);
        DesignerBunchT = 1.0 / effi_bunch;
        ts = CurrentTime() + DesignerBunchT*60;

        CheckClock(ts,A_CloseT);
        if(!OffProduction){
            Schedule(ts, e);
            Flower = Flower - 12;
            Designer--;
            Bunch_Designer++;
        }
    } else {
        e->EventType = DESIGNER_PREPROCESS;
        effi_designer = uniform(range_effi_designer[0],range_effi_designer[1]);
        DesignerProcessT = 1.0 / effi_designer;
        ts = CurrentTime() + DesignerProcessT*60;

        CheckClock(ts,A_CloseT);
        if(!OffProduction){
            Schedule(ts, e);
            Designer--;
            Process_Designer++;
        }
    }
}


void designer_bunch(EventData *a){

    if(a->EventType != DESIGNER_BUNCH) {
        fprintf(stderr, "Incorrect Event Type: NOT DESIGNER_BUNCH\n");
        exit(1);
    } else {printf("designer bunch\n");}

    //int BP = (int) (effi_designer * DesignerProcessT);
    Bunch ++;
    A_Bunch ++;

    Bunch_Designer--;
    Designer++;

    EventData* e = (EventData*)malloc(sizeof(EventData));
    if(e == NULL){fprintf(stderr, "invalid malloc\n"); exit(1);}
    double ts;

        if (Flower >= 12) {
        e->EventType = DESIGNER_BUNCH;
        effi_bunch = uniform(range_effi_bunch[0],range_effi_bunch[1]);
        DesignerBunchT = 1.0 / effi_bunch;
        ts = CurrentTime() + DesignerBunchT*60;

        CheckClock(ts,A_CloseT);
        if(!OffProduction){
            Schedule(ts, e);
            Flower = Flower - 12;
            Designer--;
            Bunch_Designer++;
        }
    } else {
        e->EventType = DESIGNER_PREPROCESS;
        effi_designer = uniform(range_effi_designer[0],range_effi_designer[1]);
        DesignerProcessT = 1.0 / effi_designer;
        ts = CurrentTime() + DesignerProcessT*60;

        CheckClock(ts,A_CloseT);
        if(!OffProduction){
            Schedule(ts, e);
            Designer--;
            Process_Designer++;
        }
    }

}


void flower_disposal(EventData *a){
	double c1 = 0.1;
	double c2 = 0.2;

    Spare_Bunch += A_Bunch;
	Spare_Flower += A_Flower;

	Cost_Disposal = c1*Spare_Flower + c2*Spare_Bunch;
}


void A_Ship_to_C(EventData *a){

    if(a->EventType != A_SHIP_TO_C) {
        fprintf(stderr, "Incorrect Event Type: NOT A_SHIP_TO_C\n");
        exit(1);
    } else {printf("A ship to C\n");}

	C_Flower = C_Flower + ASC_Flower;
	C_Bunch = C_Bunch + ASC_Bunch;
	StorageRunOut = false;
}


void A_Ship_to_M(EventData *a){

    if(a->EventType != A_SHIP_TO_M) {
        fprintf(stderr, "Incorrect Event Type: NOT A_SHIP_TO_M\n");
        exit(1);
    } else {printf("A ship to M\n");}

	M_Flower = M_Flower + ASM_Flower;
	M_Bunch = M_Bunch + ASM_Bunch;
}


void M_Ship_to_C(EventData *a){

    if(a->EventType != M_SHIP_TO_C) {
        fprintf(stderr, "Incorrect Event Type: NOT M_SHIP_TO_C\n");
        exit(1);
    } else {printf("M ship to C\n");}

	C_Flower = C_Flower + MSC_Flower;
	C_Bunch = C_Bunch + MSC_Bunch;
	StorageRunOut = false;
}


void Ship_Back(EventData *a){

    if(a->EventType != SHIP_BACK) {
        fprintf(stderr, "Incorrect Event Type: NOT SHIP_BACK\n");
        exit(1);
    } else {printf("Ship_Back\n");}

	double ts;
	double c3 = 0.1;
	double c4 = 0.2;
	double Ship_Back_Time = 3600.0;

	if (M_Flower > 0 || M_Bunch > 0){
		Spare_Flower += M_Flower;
		Spare_Bunch += M_Bunch;
		Cost_Ship_Back = c3*M_Flower + c4*M_Bunch;
		M_Flower = 0;
		M_Bunch = 0;

		EventData *e = malloc(sizeof(EventData));
		e->EventType = FLOWER_DISPOSAL;
		ts = CurrentTime() + Ship_Back_Time;
		Schedule(ts, e);
	}

}


void arrival(EventData *a){

    if(a->EventType != ARRIVAL) {
        fprintf(stderr, "Incorrect Event Type: NOT ARRIVAL\n");
        exit(1);
    } else {printf("arrival\n");}

    EventData* e = (EventData*)malloc(sizeof(EventData));
    if(e == NULL){fprintf(stderr, "invalid malloc\n"); exit(1);}
    double ts;
    double interval = randexp(300.0/A);       // customer arrival rate follows Poisson Distribution
    if (interval < 0) { printf("wrong interval\n"); exit(1);}
    ts = CurrentTime() + interval;           // next customer will arrive after 60.0/A seconds
    CheckClock(ts,C_CloseT);
    if(!OffSale) {
        e->EventType = ARRIVAL;
        Schedule(ts,e);                          // schedule next customer arrival
    }


    if (StorageRunOut) {
        BuyWhat();
        Ideal_Income += PF * FlowerBuy + PB * BunchBuy;
    } else {
        EventData* d = (EventData*)malloc(sizeof(EventData));
        if(d == NULL){fprintf(stderr, "invalid malloc\n"); exit(1);}
        d->EventType = CHECK_OUT;
        double look_time = randexp(180.0);
        ts = CurrentTime() + look_time;
        Schedule(ts,d);
    }

}


void check_out(EventData *a) {

    if(a->EventType != CHECK_OUT) {
        fprintf(stderr, "Incorrect Event Type: NOT CHECK OUT\n");
        exit(1);
    } else {printf("check out\n");}

    BuyWhat();

    Ideal_Income = PF * FlowerBuy + PB * BunchBuy;

    CheckStorage();

    Actual_Income += PF * FlowerSell + PB * BunchSell;

    if (StorageRunOut && !HaveCalled) {
        EventData* e = (EventData*)malloc(sizeof(EventData));
        if(e == NULL){fprintf(stderr, "invalid malloc\n"); exit(1);}
        double ts;
        e->EventType = CALL_SUPPLEMENT;
        double interval = 0.000001;       // customer arrival rate follows Poisson Distribution
        ts = CurrentTime() + interval;           // next customer will arrive after 60.0/A seconds
        Schedule(ts,e);                          // schedule next customer arrival        HaveCalled = true;
    }

}


void call_supplement(EventData *a){

    if(a->EventType != CALL_SUPPLEMENT) {
        fprintf(stderr, "Incorrect Event Type: NOT CALL SUPPLEMENT\n");
        exit(1);
    } else {printf("call supplement\n");}

    if(!StorageRunOut && CurrentTime() != 13.5 * 3600) {
        printf("Condition Wrong: StorageRunOut is not true\n");
        exit(1);
    }

    if (HaveCalled) {
        printf("Already Called, this call will not be dealt with\n");
    } else {

        HaveCalled = true;

        MakeOrder();

        bool ShipFromA = false;

        if(Order_Flower >= M_Flower) {
            MSC_Flower = M_Flower;
            ASC_Flower = Order_Flower - MSC_Flower;
            ShipFromA = true;
        } else {
            MSC_Flower = Order_Flower;
        }

        if(Order_Bunch >= M_Bunch) {
            MSC_Bunch = M_Bunch;
            ASC_Bunch = Order_Bunch - MSC_Bunch;
            ShipFromA = true;
        } else {
            MSC_Bunch = Order_Bunch;
        }

        EventData* d = (EventData*)malloc(sizeof(EventData));
        if(d == NULL){fprintf(stderr, "invalid malloc\n"); exit(1);}
        double ts;
        d->EventType = M_SHIP_TO_C;
        double interval = 3600.0;       // customer arrival rate follows Poisson Distribution
        ts = CurrentTime() + interval;           // next customer will arrive after 60.0/A seconds
        Schedule(ts,d);                          // schedule next customer arrival
        M_Flower = M_Flower - MSC_Flower;
        M_Bunch = M_Bunch - MSC_Bunch;

        if (ShipFromA) {
            EventData* e = (EventData*)malloc(sizeof(EventData));
            if(e == NULL){fprintf(stderr, "invalid malloc\n"); exit(1);}
            e->EventType = A_SHIP_TO_C;
            double interval = 2 * 3600.0;       // customer arrival rate follows Poisson Distribution
            ts = CurrentTime() + interval;           // next customer will arrive after 60.0/A seconds
            Schedule(ts,e);                          // schedule next customer arrival
            A_Flower = A_Flower - ASC_Flower;
            A_Bunch = A_Bunch - ASC_Bunch;
        }
    }
}


void retail(EventData *a){

    if(a->EventType != RETAIL) {
        fprintf(stderr, "Incorrect Event Type: NOT RETAIL\n");
        exit(1);
    } else {printf("retail\n");}

    Ideal_Income += PF * C_Flower + PB * C_Bunch;
    Actual_Income += 0.5 * (PF * C_Flower + PB * C_Bunch);

    totalBunchSell += C_Bunch;
    totalFlowerSell += C_Flower;

    Day ++;

}

//Company A
/////////////////////////////////////////////////////////
void CheckClock(double t, double CloseT){

    if (CloseT == (3600.0*8) && t>=CloseT){
        OffProduction = true;
    }
    if (CloseT == (3600.0 * 18) && t>=CloseT){
        OffSale = true;
    }

}
////////////////////////////////////////////////////////


//Company B
/////////////////////////////////////////////////////////

////////////////////////////////////////////////////////


//Company C
///////////////////////////////////////////////////////
void BuyWhat() {

    rand();
    double q = urand();
    if(q < 0.5) {
        buy_flower = true;
        FlowerBuy = HowMany(5);
        BunchBuy = 0;}
    else {
        buy_flower = false;
        BunchBuy = HowMany(2);
        FlowerBuy = 0;
    }
}

int HowMany (int a) {

    int amount = 1 + rand() % a;
    return amount;
}

void CheckStorage() {

    if(buy_flower) {
        BunchSell = 0;
        if(C_Flower >= FlowerBuy) {
            FlowerSell = FlowerBuy;
        } else {
            FlowerSell = C_Flower;
        }
    } else {
        FlowerSell = 0;
        if(C_Bunch >= BunchBuy) {
            BunchSell = BunchBuy;
        } else {
            BunchSell = 0;
        }
    }

    C_Flower -= FlowerSell;
    C_Bunch -= BunchSell;

    totalBunchSell += BunchSell;
    totalFlowerSell += FlowerSell;

    if (C_Flower + C_Bunch == 0) {
        StorageRunOut = true;
    }
}

void MakeOrder() {

    double StartTime;
    double EndTime;
    double SaleTime;
    double SpareTime;
    StartTime = 10 * 3600.0;
    EndTime = 18 * 3600.0;
    SaleTime = CurrentTime() - StartTime;
    SpareTime = (EndTime - CurrentTime())-1.5*3600;

    Order_Flower = (int) (totalFlowerSell / SaleTime * SpareTime);
    Order_Bunch = (int)(totalBunchSell / SaleTime * SpareTime);

}
////////////////////////////////////////////////////////////////////////////

void presentTime(int t){
    int h;
    int m;
    int s;
    h = t/3600;
    m = (t%3600)/60;
    s = (t%3600)%60;
    if(h<10) {
        if(m<10){
            if(s<10){printf("0%d:0%d:0%d\n",h,m,s);}
            else {printf("0%d:0%d:%d\n",h,m,s);}
        } else{
            if(s<10){printf("0%d:%d:0%d\n",h,m,s);}
            else {printf("0%d:%d:%d\n",h,m,s);}
        }
    } else {
            if(m<10){
            if(s<10){printf("%d:0%d:0%d\n",h,m,s);}
            else {printf("%d:0%d:%d\n",h,m,s);}
        } else{
            if(s<10){printf("%d:%d:0%d\n",h,m,s);}
            else {printf("%d:%d:%d\n",h,m,s);}
        }
    }
}

void EventHandler(void *data){

    EventData* e;

	//coerce event type
	e = (EventData*) data;
	if (e==NULL) printf("EVENT HANDLER ERROR! (malloc)\n");

	if (e->EventType == WORKER_PREPROCESS) worker_preprocess(e);
	if (e->EventType == DESIGNER_PREPROCESS) designer_preprocess(e);
	if (e->EventType == DESIGNER_BUNCH) designer_bunch(e);
    if (e->EventType == FLOWER_DISPOSAL) flower_disposal(e);
    if (e->EventType == A_SHIP_TO_C) A_Ship_to_C(e);
	if (e->EventType == A_SHIP_TO_M) A_Ship_to_M(e);
	if (e->EventType == M_SHIP_TO_C) M_Ship_to_C(e);
    if (e->EventType == SHIP_BACK) Ship_Back(e);
    if (e->EventType == ARRIVAL) arrival(e);
	if (e->EventType == CHECK_OUT) check_out(e);
	if (e->EventType == CALL_SUPPLEMENT) call_supplement(e);
    if (e->EventType == RETAIL) retail(e);
}



int main (){

    srand((unsigned)time(NULL));

    EventData* a = (EventData*)malloc(sizeof(EventData));
    if(a == NULL){fprintf(stderr, "invalid malloc\n"); exit(1);}
    EventData* b = (EventData*)malloc(sizeof(EventData));
    if(b == NULL){fprintf(stderr, "invalid malloc\n"); exit(1);}
    EventData* c = (EventData*)malloc(sizeof(EventData));
    if(c == NULL){fprintf(stderr, "invalid malloc\n"); exit(1);}
    EventData* d = (EventData*)malloc(sizeof(EventData));
    if(d == NULL){fprintf(stderr, "invalid malloc\n"); exit(1);}
    EventData* e = (EventData*)malloc(sizeof(EventData));
    if(e == NULL){fprintf(stderr, "invalid malloc\n"); exit(1);}
    EventData* f = (EventData*)malloc(sizeof(EventData));
    if(f == NULL){fprintf(stderr, "invalid malloc\n"); exit(1);}
    EventData* g = (EventData*)malloc(sizeof(EventData));
    if(g == NULL){fprintf(stderr, "invalid malloc\n"); exit(1);}
    EventData* h = (EventData*)malloc(sizeof(EventData));
    if(h == NULL){fprintf(stderr, "invalid malloc\n"); exit(1);}

    range_effi_worker = (double*)malloc(sizeof(double)*2);
    range_effi_designer = (double*)malloc(sizeof(double)*2);
    range_effi_bunch = (double*)malloc(sizeof(double)*2);

    range_effi_worker[0] = 0.33;
    range_effi_worker[1] = 0.5;
    range_effi_designer[0] = 0.5;
    range_effi_designer[1] = 1.0;
    range_effi_bunch[0] = 0.1;
    range_effi_bunch[1] = 0.2;

    double ts = 0.0;

	int Total_Salary = 0;


	Total_Salary = Worker*30 + Designer*80;
	printf("The total salary is %d\n", Total_Salary);

    e->EventType = WORKER_PREPROCESS;
    int numWorker = Worker;
    for (int i = 0; i<numWorker; i++){
        effi_worker = uniform(0.333,0.5);
        WorkerProcessT = 1.0 / effi_worker;
        ts = (WorkerProcessT*60) + 2*3600.0;
        Schedule(ts,e);
        Worker--;
        Process_Worker++;
    }

    d->EventType = DESIGNER_PREPROCESS;
    int numDesigner = Designer;
    for (int i = 0; i<numDesigner; i++) {
        effi_designer = uniform(0.5,1.0);
        DesignerProcessT = 1 / effi_designer;
        ts = (DesignerProcessT*60) + 2*3600.0;
        Schedule(ts,d);
        Designer--;
        Process_Designer++;
    }

    ASC_Bunch = First_Order_Bunch;
    ASC_Flower = First_Order_Flower;
    c->EventType = A_SHIP_TO_C;
    ts = 8*3600.0;
    Schedule(ts,c);

    ASM_Bunch = 0.3*First_Order_Bunch;
    ASM_Flower = 0.3*First_Order_Flower;
    b->EventType = A_SHIP_TO_M;
    ts = 8*3600.0;
    Schedule(ts,b);

    a->EventType = ARRIVAL;
    ts = 10*3600.0;
    Schedule(ts,a);

    f->EventType = RETAIL;
    ts = 18*3600.0;
    Schedule(ts,f);

    g->EventType = SHIP_BACK;
    ts = 18*3600.0;
    Schedule(ts,g);

    h->EventType = CALL_SUPPLEMENT;
    ts = 13.5*3600;
    Schedule(ts,h);

	RunSim(3600.0*24);

	printf("\nFlowers: %d, Bunches: %d\n", Flower, Bunch);

}
