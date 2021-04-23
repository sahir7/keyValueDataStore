#include<iostream>
#include<map>
#include<vector>
#include <fstream>
#include<thread>
#include <string>
#include <unordered_map>
#include<time.h>
#include <stdio.h>
#include <stdint.h>
#include<utility>
#include<thread>
#include<string>
#include<fstream>

// using nlohman jsoncpp library for json object in c++

#include "json.hpp"

using namespace std;

// using nlohmann json library to read write and parse json object in c++
using json = nlohmann::json;


class dataBase{

//use unordered_map to store and access data
        unordered_map<string,pair<json,time_t>> m;
        public:

        void create(string key, json value,int expiretime);
        void read(string key);
        void dElete(string key);

};


//function to check every elements in string are alphabet
bool isLetter(string s){
        int n=s.length();
        for(int i=0;i<n;i++){
                if(!isalpha(s[i])){
                    return false;
                    }
        }
        return true;
}


//for create operation,
//provide key ,value And expiretime for key
void dataBase::create(string key,json value,int expiretime){

//     if key already exist return error
        if(m.find(key)!=m.end()){
                cout<<"error : key already exist"<<endl;
        }
        else{
                if(isLetter(key)){

                  //constraints for file size less than 1GB and Jasonobject value less than 16KB
                        if(m.size() < (1024*1024*1024) && value.size()<=(16*1024*1024)){

                                pair<json,time_t> p;
                                p={value,time(nullptr)+expiretime};


                                if(key.length()<=32){ //constraints for input key_name capped at 32chars
                                        m.insert({key,p});
                                }
                        }
                        else{
                                cout<<"error : file size should be less than 1GB"<<endl;
                        }
                }
                else{
                        cout<<"error : key only contains alphabet"<<endl;
                }
        }
}

//for read operation
//use syntax "read(key_name)"

void dataBase::read(string key){


        if(m.find(key)==m.end()){   //return error if key is not exist in dataBase
                cout<<"error : key does not exist in data base"<<endl;
        }
        else{
                time_t a=m[key].second;
                if(time(nullptr) < a){  //compare present time to expire time
                        cout<<m[key].first<<endl;

                }
                else{  //Once the Time-To-Live for a key has expired, the key will no longer be available for Read  operations
                        cout<<"error : time to live off "<<key<<" has expired"<<endl;
                }

        }

}



//for delete operation
//use syntax dElete(key)

void dataBase::dElete(string key){
         if(m.find(key)==m.end()){  //return error message if key is not exist in dataBase
                cout<<"error:  key does not exist in data base"<<endl;
        }
        else{
                time_t a=m[key].second;
                if(time(nullptr) < a){  // compare present and expiretime
                        m.erase(key);
                        cout<<key<<"is deleted is deleted succesfully "<<endl;
                }
                else{ //Once the Time-To-Live for a key has expired, the key will no longer be available for Delete operations
                        cout<<"error : time to live off "<<key<<"  has expired"<<endl;
                }

        }

}
int main(){


        dataBase d;  // create dataBase object


       // for reading json file
       std::ifstream i("section.json");
       json j1=json::parse(i);

       // json object format accding to nhlohman jsoncpp library
       json j2 = {  {"Name", "Mohd"}, {"Class","Btech" },{"Roll no", "1702"}  };


       // create SectionA key
	d.create("SectionA",j1,3);
        d.read("SectionA"); //read SectionA key



	d.create("SectionB",j2,1);

        d.read("SectionB");

	d.read("SectionC");  // read non existing key


	//delete
	d.dElete("SectionC"); //dElete non-existing key


	d.dElete("SectionA"); // delete existing key


       // multithreading

        thread t1(&dataBase::read,&d,"SectionA");  //thread t1 for read SectionA key


        thread t2(&dataBase::read,&d,"SectionB"); // thread t2 for read SectionB key

       // read SectionB key after 1 second
       this_thread::sleep_for(chrono::seconds(1));
        thread t3(&dataBase::read,&d,"SectionB");// thread t3

      // join  threads with main thread
        t2.join();
        t1.join();
        t3.join();



        return 0;
}
