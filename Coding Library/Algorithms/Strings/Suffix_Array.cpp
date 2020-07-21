#include<string>
#include<algorithm>
#include<iostream>

using namespace std;

int charToIndex(char c) // change accordingly
{
    if(c == '$')
        return 0;
    else return c-96;
}
void SortCharacters(string* s,int* order) //counting sort
{
    int count[27] = {0};
    for (int i = 0; i < s->length(); i++)
        count[charToIndex(s->at(i))]++;
    for (int i = 1; i < 27; i++)
        count[i] += count[i-1];
    for (int i = s->length()-1; i >= 0; i--)
    {
        count[charToIndex(s->at(i))]--;
        *(order + count[charToIndex(s->at(i))]) = i;
    }
    
}
void ComputeCharClass(string* s,int* order,int* Equi_Class) // assing Equivalence Class to Partial Cyclic Shifts of L = 1
{
    *(Equi_Class+*order) = 0;
    for (int i = 1; i < s->length(); i++)
    {
        if(s->at(*(order+i)) != s->at(*(order+i-1)))
            *(Equi_Class+*(order+i)) = *(Equi_Class + *(order + i - 1)) + 1;
        else
        *(Equi_Class+*(order + i)) = *(Equi_Class + *(order + i - 1));
    }
    
}
void SortDoubled(string* s,int L,int* order,int* Equi_Class,int* newOrder) // updating order from L->2L
{
    int count[s->length()] = {0};
    for (int i = 0; i < s->length(); i++)
    {
        count[*(Equi_Class + i)]++;
    }
    for (int i = 1; i < s->length(); i++)
    {
        count[i] += count[i-1];
    }
    for (int i = s->length() - 1; i >= 0; i--)
    {
        int start = (*(order + i) - L + s->length()) % s->length();
        count[*(Equi_Class + start)]--;
        *(newOrder + count[*(Equi_Class + start)]) = start;
    }
     
}
void updateClass(string* s,int* newOrder,int* Equi_Class,int L,int* newEqui_Class)
{
    *(newEqui_Class + *newOrder) = 0;
    for (int i = 1; i < s->length(); i++)
    {
        int cur = *(newOrder + i);
        int prev = *(newOrder + i -1);
        int curMid = (cur + L) % s->length();
        int prevMid = (prev + L) % s->length();
        if((*(Equi_Class + cur) != *(Equi_Class + prev))||(*(Equi_Class + curMid) != *(Equi_Class + prevMid)))
            *(newEqui_Class + cur) = *(newEqui_Class + prev) + 1;
        else
            *(newEqui_Class + cur) = *(newEqui_Class + prev);       
    }
    
}
void BuildSuffixArray(string* s,int* order)//(S SHOULD HAVE $ IN THE END)order stores starting indexes of suffixes and length denotes number of differnet characters present in the string
{
    if(s->at(s->length()-1) != '$')
        s->append("$");
    int Equi_Class[s->length()];
    SortCharacters(s,order);
    ComputeCharClass(s,order,Equi_Class);
    int L = 1;
    while(L < s->length())
    {
        int newOrder[s->length()];
        int newEqui_Class[s->length()];
        SortDoubled(s,L,order,Equi_Class,newOrder);
        updateClass(s,newOrder,Equi_Class,L,newEqui_Class);
        copy(newOrder,newOrder + s->length(),order);
        copy(newEqui_Class,newEqui_Class + s->length(),Equi_Class);
        L = L*2;
    }
}
void buildLCParray(string* s,int* order,int* lcp) // lcp[i] = LCP(order[i],order[i+1])
{
    int rank[s->length()];
    for (int i = 0; i < s->length(); i++)
    {
        rank[*(order + i)] = i;
    }
    int k=0;
    for (int i = 0; i < s->length(); i++)
    {
        if(rank[i] == s->length()-1)
        {
            k=0;
            continue;
        }
        int j = *(order + rank[i] + 1);
        while(i + k < s->length() && j + k < s->length() && s->at(i+k) == s->at(j+k))
            k++;
        lcp[rank[i]] = k;
        k = max(0,k-1);
    }    
}