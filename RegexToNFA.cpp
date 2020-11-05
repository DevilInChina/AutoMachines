#include <bits/stdc++.h>
using namespace std;
#define EPSILON '-'
class Status {
private:
public:
    int indx = -1;
    map<char, vector<int> >Next;
};
class NFA {
private:
    int beg;
    int acc;
    vector<Status> StatusStore;
    vector<int> getClosure(int indx){
        set<int>ret;
        queue<int>que;
        que.push(indx);
        ret.insert(indx);
        while (!que.empty()){
            int c = que.front();
            que.pop();
            for(auto it:(StatusStore[c].Next[EPSILON])){
                if(!ret.count(it)){
                    que.push(it);
                    ret.insert(it);
                }
            }
        }
        return vector<int>(ret.begin(),ret.end());
    }
    bool ifacc(const string &s, int CurLen, int indx,bool showGraph=false) {
        if (CurLen > s.size() || indx == -1)return false;

        bool ret = false;
        auto vecClosure = getClosure(indx);

        for (auto S:vecClosure) {

            if (S == acc && CurLen == s.size())return true;
            if (CurLen >= s.size())continue;

            auto it = StatusStore[S].Next.find(s[CurLen]);

            if (it == StatusStore[S].Next.end())continue;

            for (auto its:it->second) {
                ret = ret | ifacc(s, CurLen + 1, its,showGraph);
                if(showGraph) {
                    cout << indx << "-->|" << s[CurLen] << "," << (ret ? "true" : "false") << "|" << its << endl;
                }
                if (ret)return ret;
            }
        }
        return ret;
    }

    void addStatus(int from,int to,char c){
        StatusStore[from].Next[c].push_back(to);
    }
    int addEmptyStatus(){
        StatusStore.emplace_back(Status());
        StatusStore.back().indx = (int)StatusStore.size()-1;
        return (int)StatusStore.size()-1;
    }
    void addEpOnTop(stack<int>&statusStack,int from){
        int temp = addEmptyStatus();
        statusStack.push(temp);
        addStatus(from,temp,EPSILON);
    }
    bool dealOperatorStar(int i,const string &str,stack<int>&statusStack) {
        if (i < str.size()  && str[i] == '*') {
            int from = statusStack.top();
            statusStack.pop();
            int to = statusStack.top();
            int temp = addEmptyStatus();
            statusStack.pop();
            addStatus(from,to,EPSILON);
            addStatus(to,temp,EPSILON);
            statusStack.push(to);
            statusStack.push(temp);
            return true;
        }
        return false;
    }
    void dealOperatorOr(stack<int>&statusStack,stack<char>&operatorStack,stack<int> &orStack ){
        int newStatus = addEmptyStatus();
        int fir = orStack.top();
        orStack.pop();
        int sec = statusStack.top();
        statusStack.pop();
        addStatus(fir,newStatus,EPSILON);
        addStatus(sec,newStatus,EPSILON);
        statusStack.push(newStatus);
        operatorStack.pop();

    }
public:
    /// (1|0)*(1|-)(1|0)*
    explicit NFA(const string &s) {
        addEmptyStatus();
        addEmptyStatus();
        beg = 0;
        acc = 1;
        stack<int> statusStack;
        stack<int> leftMark;
        stack<char> operatorStack;
        stack<int> orStack;
        statusStack.push(beg);
        for (int i = 0 ; i < s.size() ;  ++i){
            char it = s[i];
            switch (it) {
                case '(': {
                    operatorStack.push('(');
                    addEpOnTop(statusStack,statusStack.top());
                    leftMark.push(statusStack.top());
                }
                    break;
                case ')': {
                    int cur = statusStack.top();
                    while (!statusStack.empty() && !leftMark.empty()
                           && statusStack.top()!=leftMark.top()){
                        //cout<<"pop "<<statusStack.top()<<endl;
                        statusStack.pop();
                    }
                    leftMark.pop();
                    statusStack.push(cur);
                    operatorStack.pop();
                    bool hasStar = dealOperatorStar(i+1,s,statusStack);
                    if(!operatorStack.empty() && operatorStack.top()=='|'){

                        statusStack.pop();
                        statusStack.pop();
                        statusStack.push(cur);
                        dealOperatorOr(statusStack,operatorStack,orStack);
                    }

                }break;
                case '*': {

                }
                    break;
                case '|': {
                    int old_top = statusStack.top();
                    orStack.push(old_top);
                    statusStack.pop();
                    statusStack.pop();
                    operatorStack.push('|');
                }
                    break;
                default: {
                    bool hasOr = !operatorStack.empty() && operatorStack.top()=='|';

                    addEpOnTop(statusStack, statusStack.top());

                    int temp = addEmptyStatus();
                    addStatus(statusStack.top(),temp,it);
                    statusStack.push(temp);
                    dealOperatorStar(i+1,s,statusStack);
                    if(hasOr){
                        dealOperatorOr(statusStack,operatorStack,orStack);
                    }
                }
                    break;
            }

        }
        addStatus(statusStack.top(),acc,EPSILON);
    }

    bool match(const string &s , bool showGraph = false) {
        if(showGraph) {
            cout << "```mermaid" << endl;
            cout << "graph LR" << endl;
        }
        return ifacc(s, 0, beg,showGraph);
    }
    void showSelf(){
        for(auto &it :StatusStore){

            for(auto &its:it.Next){

                for(auto itss:its.second){
                    cout<<it.indx<<"-->|"<<its.first<<"|"<<itss<<endl;
                }
            }

        }
    }
};
//(0|1)*0011((0|1)*1(0|1)*)*
//(0|1)*1(0|-)1(0|1)*

int main() {
    cout<<"```mermaid"<<endl;
    cout<<"graph LR"<<endl;
    NFA S("(0|1)*1(0|-)1(0|1)*");
    S.showSelf();
    string str;
    while (cin >> str) {
        cout << S.match(str,true)<<endl;
    }
    return 0;
}
