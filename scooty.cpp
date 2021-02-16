/**********************************************************************
Matr.-Nr:                     3099523
Nachname/Surname:             KAFO TSAKOU
Vorname/Given name:          GERMAN DELAURE
Uni-Email:                    german.kafo-tsakou@stud.uni-due.de
Studiengang/Course of studis: Computer Engineering software
**********************************************************************/
#include<iostream>
#include<iomanip>
#include<cstring>
#include<string>
using namespace std;


class DayOfYear
{
private:
    unsigned short int dayOfYear, year;
    static const int days[];

public:
    DayOfYear():
        dayOfYear(1),
        year(365)
        {}
    DayOfYear( unsigned short int day, unsigned short int month, unsigned short int y){

        int m=days[month-1];

        dayOfYear=m+day;
        year=y;

    }
    unsigned int get_dayOfYear(){
    return dayOfYear;
    }
    DayOfYear& operator++(int){
     DayOfYear d;
    if (dayOfYear>=365){
        dayOfYear=1;
        year++;
    }else{
        dayOfYear++;
    }
     return *this;
    }

    friend istream& operator>>(istream& in, DayOfYear& b ){
    unsigned short int day, month, year;
    char c1, c2;
    in >> year >> c1 >> month >> c2 >> day;
    b=DayOfYear(day, month, year);

    return in;

    }
    friend ostream& operator<<(ostream& out,const DayOfYear& b){
        int i,month ,day;

        for(i=0;i<=13;i++){
            if(b.dayOfYear>b.days[i] && b.dayOfYear<=b.days[i+1]){
                day=b.dayOfYear-b.days[i];
                month=i+1;
                break;
            }else{
                continue;
            }
        }

        out<<setw(4)<<b.year<<"-"<<setw(2)<<setfill('0')<<month<<"-"<<setw(2)<<setfill('0')<<day;
        return out;
        }

};
 const int DayOfYear::days[13]={0,31,59,90,120,151,181,212,243,273,304,334,365};

class Vehicle
{
    private:
        const int no;
        string model;
        float price24h;
    public:
        Vehicle(int num,string mod, float p24):
            no(num),
            model(mod),
            price24h(p24){}
        virtual ~Vehicle(){cout<<"destructor Vehicle is done"<<endl;}
        int get_no(){
            return no;
        }
        string get_model(){
            return model;
        }
        float get_price(int n){
            return n*price24h;
        }
        virtual void print()=0;

};

class Bike: public Vehicle
{
    public:
        Bike (int a,string s):
            Vehicle(a, s, 9.99)
            {}
        virtual void print(){
            cout<<get_no()<<" : "<<get_model()<<" (Bike) "<<endl;
        }
};

class EMotorVehicle: public Vehicle
{
    public:
        EMotorVehicle(int a, string s, float n):
            Vehicle(a,s,n)
            {}
        virtual bool is_streetLegal()=0;

        virtual void print(){
             cout<<get_no()<<" : "<<get_model()<< (is_streetLegal()? "":"(not street legal)");

        }
};

class Ebike: public EMotorVehicle
{
    public:
        Ebike(int a, string s, float n=29.99):
            EMotorVehicle(a,s,n)
            {}
        virtual bool is_streetLegal(){
            return true;
            }

        virtual void print(){
            EMotorVehicle::print();
            cout<<" (EBike) "<<endl;

        }
};

class EScooter: public EMotorVehicle
{
    private:
        bool streetLegal;
    public:
        EScooter(int a, string s, float n=19.99,bool b=false):
            EMotorVehicle(a,s,n),
            streetLegal(b)
            {}
        virtual bool is_streetLegal(){
            return streetLegal;
            }

        virtual void print(){
            EMotorVehicle::print();
            cout<<" (EScooter) "<<endl;

        }
};

class Rental
{
private:
    const int no;
    DayOfYear from;
    int days;
    string customer;
    static int last_no;
public:
    Rental  (string s, DayOfYear d,int r=1);
    int get_days();
    DayOfYear get_from();
    DayOfYear get_until();
    void print();

};
int Rental::last_no=1;
Rental::Rental(string s, DayOfYear d,int r):
    customer(s),
    from(d),
    days(r),
     no(last_no)
{
last_no++;

}
int Rental::get_days(){
    return days;
}
DayOfYear Rental::get_from(){
    return from;
}
DayOfYear Rental::get_until(){
    int i;
    DayOfYear until=from;
    for(i=0;i<days;i++){
        until++;
    }
    return until;
}

void Rental::print(){
    cout<<get_from()<<" to "<<get_until()<<" , rental no. "<<no<<" for "<<customer<<" , total: ";
}



class Schedule
{
private:
    Vehicle *vehicle;
    Rental *year[365];
    Schedule *next;

public:
    Schedule( Vehicle *v) :
        vehicle(v),
        next(NULL)
      {  for(int i=0;i<365;i++)
           {
               year[i] = NULL;}
      }

    Vehicle *get_vehicle(){
        return vehicle;
    }
    Schedule *get_next(){
        return next;
    }
    Schedule *set_next(Schedule *n){
        next=n;
        return next;

    }
    bool isFree(DayOfYear d,int n){
        for(int i=(d.get_dayOfYear()-1);i<d.get_dayOfYear()+n;i++){
            if (year[(d.get_dayOfYear()) - 1] != NULL)
            {
                return false;
                break;
            }

        }
        return true;
    }

    float book(string p,DayOfYear d,int n)
    {DayOfYear b=d;
        for(int i=(d.get_dayOfYear()-1);i<d.get_dayOfYear()+n;i++){
            year[i]=new Rental(p,b,n);
            b++;
        }
        return vehicle->get_price(n);
    }
    void print(DayOfYear d){

        vehicle->print();
        if(year[(d.get_dayOfYear())-1] != nullptr){
            year[(d.get_dayOfYear())-1] ->print();
        }

    }
    void print(){
        cout<<"SCHEDULE FOR "; vehicle->print();
        for(int i=0;i<365;i++){
            if(year[i]!=nullptr){
                year[i]->print();
                int j = year[i]->get_days();
                float f = vehicle->get_price(j);
                cout << f << " EUR" << endl;
                break;
          }  }



    }

};

class Agency
{
private:
    float profitPercent;
    float profit;
    Schedule *head,*last;
protected:
    Schedule *searchFor(int n){

            Schedule *s;
            s=head;
            while(s!=NULL){
                if(s->get_vehicle()->get_no()==n){
                    return s;
                }else{
                    s=s->get_next();
                }
            }
    }
public:
    Agency():
    profitPercent(0.2),
    profit(0.00),
    head(NULL),
    last(NULL)
     {

    }
    float get_profit(){
        return profit;
    }
    void set_profitPercentage(float f){
        profitPercent = f;
    }
    void add(Vehicle *v){

       if (v != nullptr)
        {
        if (head == nullptr)
            head =last = new Schedule(v);
        else
            last = last->set_next(new Schedule(v));
        }
       /* Schedule *f;
        f=new Schedule(v);

        //f->get_vehicle()->print();
        f->set_next(head);
        head=f;*/

    }
    bool isFree(int n,DayOfYear d,int c){
        Schedule *f =searchFor(n);
        return f->isFree(d,c);


    }

    float book(int n,string s,DayOfYear d,int c){
        Schedule *f =searchFor(n);
        float pc,pf;
        if (pc=f->book(s,d,c)){
            cout<<"";
        }else{
        cout<<"could not book";}
        pf=profitPercent*pc;
        profit=profit+pf;
        return pc;
    }


    int chooseVehicle(){
        int n;
        Schedule *s;
        s=head;
        while(s!=NULL){
            s->get_vehicle()->print();
            s=s->get_next();
        }
        cout<<"choose vehicle no: ";
        cin>>n;
        return n;
    }

    void print(DayOfYear d){
        Schedule *s;

        while (s!=NULL){

        s->print(d);
        s=s->get_next();
        }

    }
    void print(){
        Schedule *s;
        s=head;
        while (s!=NULL)

        {
            s->print();
            s=s->get_next();
        }
       cout<<"profit: "<<fixed <<setprecision(2)<<profit<<" EUR";
    }
};

int main(){
    Agency a;
    DayOfYear today(8,1,2021);
    DayOfYear tomorrow=today;
    tomorrow++;

    cout<<"SIMPLIFIED SIMULATION OF A RENTAL AGENCY"<<endl;
    cout<<"========================================"<<endl;
    bool sim=true;
    char choice;
    int num=1,b1=1,b2=1,b3=1;

    while(sim==true){
        cout<<endl<<"________________________________________"<<endl
           <<"MENUE"<<endl
            <<"A end of simulation"<<endl
            <<"B set new simulation date"<<endl
            <<"C new rental manually"<<endl
            <<"D print rental of today "<<today<<endl
            <<"E print rental of tomorrow "<<tomorrow<<endl
            <<"F print all rentals"<<endl
            <<"G print agency profit"<<endl
            <<"H set agency profit percent"<<endl
            <<"I add bikes"<<endl
            <<"J add E-bikes"<<endl
            <<"K add E-scooters"<<endl
            <<"L new rental simulation"<<endl
            <<"your choice: ";
            cin>> choice;
            choice=toupper( choice);
            switch (choice)
            {
            case 'A' :
                sim=false;

                break;
            case 'B' :
                cout<<"input date of today :";
                cin>>today;
                tomorrow=today;
                tomorrow++;
                break;
            case 'C' :
               {

                DayOfYear d;
                int c;
                string cust;
                int n=a.chooseVehicle();
                cout << "day of rental: " ;
                cin >> d;
                cout << "number of days: " ;
                cin >> c;
                if (a.isFree(n,d,c)){
                    cout << "input data vehicle renter name: " ;
                    cin >> cust;
                    cout << "booked, price for this rental: ";
                    float pr = a.book(n,cust,d,c);
                    cout <<fixed <<setprecision(2)<< pr <<" EUR "<< endl;

                }
                else {
                    cout << "sorry vehicle already booked" << endl;

            }

                break;}
            case 'D' :
                cout<<"SCHEDULE FOR "<<today<<endl;
                a.print(today);

                break;
            case 'E' :
                cout<<"SCHEDULE FOR "<<tomorrow<<endl;
                a.print(tomorrow);
                break;
            case 'F':
                a.print();
                break;
            case 'G':
                cout<<"agency profit : ";cout<<a.get_profit(); cout<<" EUR";
                break;
            case 'H' :
                int p;
                float pr;
                cout<<"agency profit percent: ";
                cin>>p;
                pr=float(p)/100.00;
                a.set_profitPercentage(pr);
                break;
            case 'I' :
                int j,i;
                cout<<"how many Bikes in this simulation? ";
                cin>>j;
                for(i=0;i<j;i++){

                    string s= "City"+to_string(b1);
                    Vehicle *v=new Bike(num,s);

                    a.add(v);

                    b1++;
                    num++;
                }



                break;
            case 'J' :

                cout<<"how many EBikes in this simulation? ";
                cin>>j;
                for(i=0;i<j;i++){

                    string s= "Trekky"+to_string(b2);
                    Vehicle *v=new Ebike(num,s);

                    a.add(v);

                    b2++;
                    num++;
                }

                break;
            case 'K' :

                cout<<"how many EScouter in this simulation? ";
                cin>>j;
                for(i=0;i<j;i++){

                    string s= "Scooty"+to_string(b3);
                    Vehicle *v=new EScooter(num,s);
                    a.add(v);

                    b3++;
                    num++;
                }

                break;

            default:
            cout<<"wrong choice , please choose a correct letter"<<endl;
                break;
            }

    }
return 0;
}


