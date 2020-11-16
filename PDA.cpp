#include <bits/stdc++.h>
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
    void showSelf()const {
        cout<<fromID<<"-->|"<<character<<","<<stackTop<<"->"<<replaceTop<<"|"<<toID<<endl;
    }

};

class PDA {
    map<tuple<int, char>, vector<Rule> > Rules;

public:
    /**
     * @brief set rules for the hole pda
     * @param n
     */
    explicit PDA(int n) {

    }

    void pushRule(int from, int to, char c, char top, char replace) {
        auto it = make_pair(from, c);
        Rules[it].emplace_back(Rule(from, to, c, top, replace));
    }

    void pushRule(int from, int to, const string &cTopReplace) {
        pushRule(from, to, cTopReplace[0], cTopReplace[1], cTopReplace[2]);
    }


    void show_self() {
        cout << "```mermaid" << endl;
        cout << "graph LR" << endl;
        for (auto &c:Rules) {
            for (auto &it:c.second)
                it.showSelf();
        }
        cout << "```" << endl;
    }

    bool deal(const string &str,int terminID) {
        string s = "#" + str;
        s.push_back('#');
        int curStatus = 1;
        vector<char> stk;

        for (int i = 0; i < s.size(); ++i) {
            char top = stk.empty() ? '#' : stk.back();
            //if(top=='+')top = '#';
            auto tup = make_pair(curStatus, s[i]);
            /*
            cout<<curStatus<<" "<<s[i]<<" "<<top<<":";
            for(auto c: stk){
                cout<<c<<' ';
            }
            cout<<'\n';
            */
            auto it = Rules.find(tup);
            if (it == Rules.end())return false;

            for (auto &c:it->second) {
                if (c.stackTop == top) {
                    curStatus = c.toID;
                    if (!stk.empty())
                        stk.pop_back();
                    if (c.replaceTop != '#')stk.push_back(c.replaceTop);

                    goto outCircle;
                }
            }
            for (auto &c:it->second) {
                if (c.stackTop == '#') {
                    curStatus = c.toID;
                    if (c.replaceTop != '#')stk.push_back(c.replaceTop);
                    goto outCircle;
                }
            }
            outCircle:;
        }
        return curStatus==terminID;
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
    s.pushRule(3,2,'b','#','b');
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
    cout<<s.deal("abbbba",7)<<endl;
    cout<<s.deal("abbbca",7)<<endl;
    cout<<s.deal("abbcca",7)<<endl;
    cout<<s.deal("abcbca",7)<<endl;
    cout<<s.deal("acbcba",7)<<endl;
    cout<<s.deal("acccba",7)<<endl;
}