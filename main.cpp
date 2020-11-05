#include <bits\stdc++.h>
using namespace std;


class Rule{
public:
    char character;
    char stackTop;
    char replaceTop;
    int fromID;
    int toID;
    Rule(int curStatus,int toStatus,char c,char top,char replaceTop){
        fromID=curStatus;
        toID = toStatus;
        character = c;
        stackTop = top;
        this->replaceTop = replaceTop;
    }
    void showSelf(){
        cout<<fromID<<"-->|"<<character<<","<<stackTop<<"->"<<replaceTop<<"|"<<toID<<endl;
    }
};

class PDA{
    vector<vector<Rule> >Rules;
public:
    /**
     * @brief set rules for the hole pda
     * @param n
     */
    explicit PDA(int n){
        while (n--){
            Rules.emplace_back(vector<Rule>() );
        }
    }
    void pushRule(int from,int to,char c,char top,char replace){
        while (from>Rules.size())Rules.emplace_back(vector<Rule>());
        Rules[from].push_back(Rule(from,to,c,top,replace));
    }
    void pushRule(int from,int to,const string&cTopReplace){
        while (from>Rules.size())Rules.emplace_back(vector<Rule>());
        Rules[from].push_back(Rule(from,to,cTopReplace[0],cTopReplace[1],cTopReplace[2]));
    }


    void show_self(){
        cout<<"```mermaid"<<endl;
        cout<<"graph LR"<<endl;
        for(auto &c:Rules){
            for(auto &s:c){
                s.showSelf();
            }
        }
        cout<<"```"<<endl;
    }
    void dealStack(int status,){

    }
    void deal(const string &str){
        string s = "#"+str;
        s+="#";

    }
};
int main(){
    PDA s(8);
    s.pushRule(1,2,'#','#','+');
    s.pushRule(2,4,'a','#','#');
    s.pushRule(2,2,'b','#','b');
    s.pushRule(2,3,'c','b','#');
    s.pushRule(2,6,'c','#','c');
    s.pushRule(2,7,'#','+','#');
    s.pushRule(3,4,'a','#','#');
    s.pushRule(3,2,'b','#','#');
    s.pushRule(3,3,'c','b','#');
    s.pushRule(3,6,'c','#','c');
    s.pushRule(3,7,"#+#");
    s.pushRule(4,4,"a##");
    s.pushRule(4,5,"bc#");
    s.pushRule(4,2,"b#b");
    s.pushRule(4,3,"cb#");
    s.pushRule(4,6,"c#c");
    s.pushRule(4,7,"#+#");
    s.pushRule(5,4,"a##");
    s.pushRule(5,5,"bc#");
    s.pushRule(5,2,"b#b");
    s.pushRule(5,6,"c#c");
    s.pushRule(5,7,"#+#");
    s.pushRule(6,4,"a##");
    s.pushRule(6,5,"bc#");
    s.pushRule(6,6,"c#c");
    s.pushRule(6,7,"#+#");

    s.show_self();
}