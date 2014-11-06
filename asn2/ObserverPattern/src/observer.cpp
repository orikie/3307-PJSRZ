#include <iostream>
#include <vector>
using namespace std;

class Subject {
    //VECTOR OF BIDDERS (this is the only instance of coupling b/w Subject and Bidder)
    vector < class Bidder * > views; 
    int currentBid;
  public:
    void attach(Bidder *obs) {
    	//ADD A NEW BIDDER
    	cout << "Subject: NEW BIDDER ADDED\n";
        views.push_back(obs);
    }
    void bid(int newbid) {
    	//NEW BID
        currentBid = newbid;
        cout << "Subject: UPDATE BID\n";
        cout << "Subject: NOTIFY BIDDERS: NEW BID: " << newbid << "\n";
        //NOTIFY BIDDERS
        notify();
    }
    int getCurrentBid() {
        return currentBid;
    }
    void notify();
};

class Bidder {
    Subject *model;
    int denom;
  public:
    Bidder(Subject *mod, int div) {
        model = mod;
        denom = div;
        //JOIN IN TO THE SUBJECT (THE BIDDING WAR)
        cout << "Observer: ADD NEW BIDDER\n";
        model->attach(this);
    }
    virtual void update() = 0;
  protected:
    Subject *getSubject() {
        return model;
    }
    int getDivisor() {
        return denom;
    }
};

void Subject::notify() {
  //BID IS MADE, UPDATE
  cout << "Subject: NOTIFYING BIDDERS...\n";
  for (int i = 0; i < views.size(); i++)
    views[i]->update();
}

class DivBidder: public Bidder {
  public:
    DivBidder(Subject *mod, int div): Bidder(mod, div){}
    void update() {
        ///GET CURRENT BID INFO
        cout << "Observer: GATHERING NEW BID INFO\n";
        int v = getSubject()->getCurrentBid();
        ///****HERE CAN DO SOMETHING WITH THE CURRENT INFORMATION (WAIT/RAISE BID) ****///
    }
};



int main() {
  Subject subj;
  DivBidder divObs1(&subj, 4); // 7. Client configures the number and
  DivBidder divObs2(&subj, 3); //    type of Bidders
  subj.bid(14);
  subj.bid(7);
}