#include <StandardCplusplus.h>
#include <serstream>
#include "hash.h"

using namespace std;

// <iostream> declares cout/cerr, but the application must define them
// because it's up to you what to do with them.
namespace std
{
  ohserialstream cout(Serial);
}

hashTable *h = new hashTable(10);

void setup(){
  Serial.begin(9600);
  cout << "hello is this the world?" << endl;
  
  int v1 = 10, v2 = 20, v3 = 30;
  
  cout << "inserting first node" << endl;
//  h->insert("first", &v1);
//  cout << "inserting second node" << endl;
//  h->insert("second", &v2);
//  cout << "inserting first node" << endl;
//  h->insert("third", &v3);
//  
//  cout << "attempting to reinsert second as v1" << endl;
//  //cout << h->insert("second", &v1) << endl;
//  
//  cout << "does hash contain 'third'?" << endl;
  //cout << h->contains("third") << endl;
}

void loop(){
  delay(1000);
}
