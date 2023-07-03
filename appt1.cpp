#include<iostream>
#include<list>
#include<cstring>
#include<string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

                                                                    // 
                                                                    // Date type

class datetype  //format: dd/mm/yy  - use operator overloading to i/p and display and compare
{
    public:
    int d;
    int m;
    int y;
    friend ostream& operator<<(ostream& ,const datetype&);
    friend fstream& operator<<(fstream& os,const datetype &d1);
    friend istream& operator>>(istream& ,datetype&);
    friend fstream& operator>>(fstream& is,datetype &d1);
    friend bool operator==(const datetype&, const datetype&);
};

ostream& operator<<(ostream& os,const datetype &d1)   //overloading o/p operator
{
    os<<d1.d<<"/"<<d1.m<<"/"<<d1.y;
    return os;
}

fstream& operator<<(fstream& os,const datetype &d1)   //overloading o/p operator
{
    os<<d1.d<<"/"<<d1.m<<"/"<<d1.y;
    return os;
}

istream& operator>>(istream& is,datetype &d1)   //overloading i/p operator
{
    is>>d1.d;
    is>>d1.m;
    is>>d1.y;
    return is;
}

fstream& operator>>(fstream& is,datetype &d1)   //overloading i/p operator
{
    is>>d1.d;
    is>>d1.m;
    is>>d1.y;
    return is;
}

bool operator==(const datetype &d1, const datetype &d2)
{
    return d1.d==d2.d && d1.m==d2.m && d1.y==d2.y;
}


class Patient
{
    public:
        char name[40];
        char contact[20];

        Patient(const char *n, const char *c)
        {
            strcpy(name, n);
            strcpy(contact, c);    
        }
};


class Timeslot
{
    public:
        int booked;
        Patient *p;
        int done;

        void addPatient()
        {
            booked=1;
            char name[20],contact[20];
            cout<<endl<<"Enter Name: ";
            cin>>name;
            cout<<"Enter contact: ";
            cin>>contact;
            p=new Patient(name, contact);
        }

        void deletePatient()
        {
            // mark done 1
            booked=0;
            done=1;
            delete p;
            p=NULL;
        }

        void showDetails()
        {
            cout<<booked
                <<endl    
                // <<p->name
                // <<p->contact
                <<done
                <<endl;
        }

};


                                                                    // Date class

class Date
{
    public:
    datetype date1;
    Timeslot t[5];
    friend Date* checkdate(list<Date> &dl,datetype da);

    void slot_init()
    {
        for(int i=0;i<5;i++)
        {
            t[i].booked=0;
            t[i].done=0;
            t[i].p = NULL;
        }
    }

    int freets(int i)                           // check if given time slot is valid

    {   
        if((i>=0 && i<5) && t[i].booked==0)
        {
            return 1;
        }
        return 0;
    }
};

list<Date *> datelist = {};      // list of dates which has booked appointments



Date* checkdate(list<Date *> &dl,datetype da)          // checks if the date object exists in the list and returns pointer to the date object
{
    int i;
    // cout<<"inside checkdate"<<endl;
    list<Date *>::iterator p = dl.begin();
    static Date *dt;
    // cout<<"before while"<<endl;
    while(p!=dl.end())
    {
        // cout<<"inside while"<<endl;
        dt = *p;
        if(dt->date1==da)
        {
            return dt;
        }
        else
            p++;
    }
    // cout<<"before NULL"<<endl;
    return NULL;
    
}


Date *findDate(datetype da)
{
    // prerequisite to display available timeslots -- entering date
    // cout<<"in findDate"<<endl;

    //da1 is the pointer to the date object for the date the user has chosen
    // cout<<"before checkdate"<<endl;
    Date *da1 = checkdate(datelist,da);         // checks if date object exists
    // cout<<"after checkdate"<<endl;
    if(da1==NULL)                               // if not, creates date object
    {
        Date *d_new=new Date;
        d_new->date1 = da;
        d_new->slot_init();
        datelist.push_back(d_new);
        da1 = d_new;
    }
    // cout<<"end of findDate"<<endl;
    return da1;
    //display available timeslots on that date

}

void fileAppend(Date *da, int i, string fileName)
{
    // only needs to be appended in the end as appointments will finish chronologically
    fstream fout;
    fout.open(fileName, ios::out | ios::app);
    fout.seekg(0, ios::end);                    // go to the end of the file
    fout << da->date1 <<','                     // date
         << i+1 <<','                           // slot hour
         << da->t[i].p->name << ','             // patient name
         << da->t[i].p->contact                 // patient contact number
         << '\n';
    fout.close();

}

void booking()
{
    datetype da;
    cout<<"Enter the date (dd mm yy)"<<endl;
    cin>>da;

    Date *da1=findDate(da);

    cout<<endl<<"Timeslots available on "<<da1->date1<<endl;
    for(int i=0;i<5;i++)                        // shows available timeslots for the day
    {
        if(da1->t[i].booked==0) 
        {
            cout<<i+1<<" - "<<i+1<<" o' clock "<<endl;
        }
    }

    int in=0;
    while(1){
        cout<<endl<<"Enter convinient time slot (or enter 0 to not book):"<<endl;          
        cin>>in;
        if(!in)                                 // if in is 0, then exit
        {                    
            cout<<"Returning to main menu"<<endl;
            return;
        }
        else if(da1->freets(in-1))
        {
            da1->t[in-1].addPatient();

            cout<<"Appointment has been booked!"<<endl<<endl;
            fileAppend(da1, in-1, "bookings.csv");                                            // write new appointment into booked file
            da1=NULL;
            return;
        }
        else
        {
            cout<<"this time slot is not available"<<endl;
        }
    }

}

void markDone()
{
    // enter date time slot
    datetype da;
    cout<<endl<<"Enter the date (dd mm yy)"<<endl;
    cin>>da;

    Date *da1=findDate(da);

    cout<<endl<<"Timeslots booked on : "<<da1->date1<<endl;
    for(int i=0;i<5;i++)                        
    {
        if(da1->t[i].booked==1) 
        {
            cout<<i+1<<" - "<<i+1<<" o' clock "<<endl;
        }
    }   

    // if time slot is booked
    int in=0;
    while(1)
    {
        cout<<endl<<"Time slot to mark done (or enter 0 to exit):"<<endl;          
        cin>>in;
        cout<<endl;
        if(!in)
        {                    // if in is 0, then exit
            cout<<"Returning to main menu"<<endl;
            return;
        }
        else if(!da1->freets(in-1))
        {
            fileAppend(da1, in-1, "done.csv");
            da1->t[in-1].deletePatient();
            cout<<"This timeslot has been marked done!"<<endl;
            return;
        }
        else
        {
            cout<<"This time slot is not available"<<endl;
        }
    }

}

void daybookings()              // friend function of date
{
    // shows days bookings
    // datelist -- list populated with booked dates

    // enter date -- using findDate function -- will return Date object

    // for the date so and so
    // if timeslot is empty
        //  print no bookings are there
    // else
        // print:
            // timeslot name contact 
    
    datetype da;
    cout<<endl<<"Enter the date (dd mm yy)"<<endl;
    cin>>da;

    Date *da1=findDate(da);

    cout<<endl<<"Timeslots booked on "<<da1->date1<<endl;
    for(int i=0;i<5;i++)                        
    {
        if(da1->t[i].booked==1) 
        {
            cout<<"Time slot: "<<i+1<<" o' clock "<<endl
                << "    Name: "<< da1->t[i].p->name<<endl 
                << "    Contact: " << da1->t[i].p->contact<<endl<<endl;
        }
    }   



}

void history(int n)               // friend function of date
{
    /*
        print n number of latest done appointments
    
        cases:
       -file doesnt exist - no history
       -if list completes before no. -print only that much
       -perfect scenario

    */
    string line;
    vector<string>v;
    fstream f("done.csv",ios::in);

    if(!f || f.eof())
    {
        cout<<endl<<"No History"<<endl;
        return;
    }
    while(getline(f,line))
    {
            v.push_back(line);
    }
    cout<<endl;
    cout<<"The last "<<n<<" bookings done are:"<<endl;
    for(int i=v.size()-1;i>=0 && n>0;i--,n--)
    {       
            istringstream iss(v[i]);
            string date , timeslot ,name ,contact;
            getline(iss,date,',');
            getline(iss,timeslot,',');
            getline(iss,name,',');
            getline(iss,contact,',');
            cout<<date<<":"<<endl<<timeslot<<" o' clock : "<<name<<", "<<contact<<" "<<endl;
    }
    cout<<endl;
    f.close();
    return;

}


void display()                  // can add default parameters for mode of display
{
        /*
            modes:      
                bookings for the day -- default
                booking history -- need to ask for number of entries to be displayed
                entire for the day
        
        
        */



}

void populate()
{
    /*
                called before printing on console
                to populate the datelist list

            for each line read from the file
                check date
                if date object doesn't exist in the list then add new date object
                 
    
    
    */

    fstream fin;
    fin.open("bookings.csv", ios::in | ios::app);
    string line, word;
    vector<string> row;
    int i;
    while(!fin.eof()){
        row.clear();
        getline(fin, line);                     // date, slot number, name, contact
        // cout<<line<<endl;

        stringstream s(line);
        while ( getline(s, word, ',') ) {
            // cout<<word<<endl;
            row.push_back(word);    
        }
        // cout<<"row 0: "<<row[0]<<endl;

        stringstream da(row[0]);
        string num;
        datetype temp;
        getline(da, num, '/');
        temp.d=stoi(num);
        getline(da, num, '/');
        temp.m=stoi(num);
        getline(da, num);
        temp.y=stoi(num);

        // cout<<"temp: "<<temp<<endl;

        Date *dobj=findDate(temp);

        i=stoi(row[1]);
        dobj->t[i-1].booked=1;
        dobj->t[i-1].done=0;
        dobj->t[i-1].p=new Patient(row[2].c_str(), row[3].c_str());


    }

}




int main()
{
    populate();

    ifstream fin;
    fin.open("intro.txt");
    string line;
    stringstream menustr;
    while (getline(fin, line)) 
    {
        // Print line (read from file) in Console
        menustr << line << endl;
    }
    fin.close();

    int choice;
    bool cont=true;
    while(cont)
    {
        cout<<menustr.str();
        cin>>choice;

        switch(choice){
            case 1:
                booking();
                break;
            
            case 2:
                markDone();
                // mark appointment done
                break;
            
            case 3:
                // view schedule -- view bookings
                daybookings();
                break;
            
            case 4:
                // view history
                int n;
                cout<<endl<<"Enter how many done bookings to display:"<<endl;
                cin>>n;
                history(n);
                break;

            case 5:
                cont=false;
                break;        

        }
    }

}