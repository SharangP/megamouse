#include <StandardCplusplus.h>
#include <string>
#include <vector>
#include <list>


using namespace std;

vector<int> v;
vector<string> strings;
list<string> strList;

void setup(void)
{
  Serial.begin(9600);
    strList.push_back("first str");
    strList.push_back("second str");
    strList.push_back("third str");
    
    strings.push_back("Hello,");
    strings.push_back("world!");
    
    v.push_back(1);
    v.push_back(14);
    
    strList.pop_front();
}

void loop(void)
{
  Serial.println("sup brah");
  Serial.println(v[0]);
  Serial.println(strings[1].c_str());
  Serial.println(strList.begin()->c_str());
  delay(1000);
}
