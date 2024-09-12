// SDP Draw
// Developed by Jamie Stade
// For the Canadian Junior Hereford Association
// Current version completed November 2023

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <set>
#include <random>
#include <algorithm>
#include <regex>
#include <limits.h>
#include <sys/stat.h>
using namespace std;


//Each bull entry in the bull list csv file is stored with this class
//Each time a female cow is matched with a bull, the name of that female cow's junior member will added to the list of juniors
class Bull{
    public:
    string bullName;
    string donatorName;
    string donatorEmail;
    int straws;
    list<string> juniors;

    Bull(string bn, string dn, string de, int s){
        bullName = bn;
        donatorName = dn;
        donatorEmail = de;
        straws = s;
    }
};

//Each female cow entry in the female list csv file is stored with this class
//bullMatch is updated with a pointer to a Bull if the Female Cow is matched with one during the draw portion
class Female{
    public:
    string femaleName;
    string memberName;
    string memberEmail;
    int rank;
    list<Bull*> bullChoices;
    Bull* bullMatch = nullptr;

    Female(string fn, string mn, string me, int r, list<Bull*> bulls){
        femaleName = fn;
        memberName = mn;
        memberEmail = me;
        rank = r;
        bullChoices = bulls;
    }
};

//hashmap with all bulls, bulls name maps to its respective Bull element
unordered_map<string, Bull> bullMap;
//vector with bull names grouped by donator's name
vector<string> orderVector;
//set containing each unique donator name
set<string> bullDonators;

//list of females grouped by member
vector<Female> femaleVector;
//set containing each unique junior member name
set<string> femaleMembers;
//indexes of all females with rank n are stored in nameCount[n-1]
vector<vector<int>> rankIndexes;

//return string with all whitespaces removed and all alphabetic characters in lowercase
string trimString (string aString){
    aString.erase(remove_if(aString.begin(), aString.end(),[](unsigned char x) { return isspace(x) || x == 0x2c || x == 0xef || x == 0xbb || x == 0xbf;}), aString.end());
    transform(aString.begin(), aString.end(), aString.begin(), ::tolower);
    return(aString);   
}

bool fileExists(const string& name){
    struct stat buffer;  
    return(stat (name.c_str(), &buffer) == 0);
}

//create or replace new output file and return ofstream
ofstream createReplaceFile (const string& name) {
    if(fileExists(name)){
        ofstream ofs;
        ofs.open(name, ofstream::out | ofstream::trunc);
        return ofs;
    }
    else{
        ofstream ofs(name, std::ofstream::out);
        return ofs;
    }
}

//throws exception if file does nt include csv extension
void csvExceptionCheck(string fileName){
    try {
            string format = fileName.substr(fileName.size()-4);
            if (format != ".csv"){
                throw fileName;
            }
    }
    catch(string input){
        cerr << "Bad Input: file name " << input << " does not include .csv extension" << endl;
        exit(EXIT_FAILURE);
    }
}

//throws exception if file does not exist
void fileExistsCheck(string fileName){
    try {
        if (!fileExists(fileName)){
            throw fileName;
        }
    }
    catch(string input){
        cerr << "Bad Input: " << input << " could not be found." << endl;
        exit(EXIT_FAILURE);
    }
}

// throws exception if string is not an integer value
int isIntegerCheck(string number, string fileName, int lineNo){
    try {
        regex integerNumber ("^[1-9][0-9]*$");
        smatch sm;
        if (!regex_match(number,sm,integerNumber)){
            throw number;
        }
        else{
            int out;
            stringstream ss;
            ss << number;
            ss >> out; 
            return out;
        }
    }
    catch(string input){
        cerr << "Bad Input: " << input << " in " << fileName << " at line " << lineNo << " is not an integer value." << endl;
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv){
    ifstream fin;
    string line;
    string currentFile;
    string bullInput, femaleInput;

    if(argc < 3){
        cout << "please enter the name of the bull list file, in csv format (eg. bull_list.csv)" << endl;
        getline(cin, bullInput);
        csvExceptionCheck(bullInput);
        fileExistsCheck(bullInput);
        
        cout << "please enter the name of the female list file, in csv format (eg. female_list.csv)" << endl;
        getline(cin, femaleInput);
        csvExceptionCheck(femaleInput);
        fileExistsCheck(femaleInput);
    }
    else{
        bullInput = argv[1];
        csvExceptionCheck(bullInput);
        fileExistsCheck(bullInput);

        femaleInput = argv[2];
        csvExceptionCheck(femaleInput);
        fileExistsCheck(femaleInput);
    }

    //read from bull input file
    fin.open(bullInput);
    currentFile = bullInput;
    for (int i=1; getline(fin, line); i++) {
        string trimmedLine = trimString(line);
        //skip empty rows and headers
        if(trimmedLine != "" && trimmedLine != "bullnamedonatornamedonatoremailstraws"){
            stringstream ss(line);
            string bn, dn, de, s;

            //separate each field
            getline(ss, bn, ',');
            getline(ss, dn, ',');
            getline(ss, de, ',');
            getline(ss, s, ',');
            int straws;
            if(trimString(s) == "unlimited"){
                straws = INT_MAX;
            }
            else{
                straws = isIntegerCheck(s, currentFile, i);
            }

            // insert record to hashmap
            Bull newBull(bn,dn,de,straws);
            bullMap.insert({bn,newBull});

            // order bull names in vector by donator name
            if(bullDonators.count(dn)){
                vector<string>::iterator it = orderVector.end();
                while(it != orderVector.begin()){
                    it--;
                    if(dn == bullMap.at(*it).donatorName){
                        it++;
                        orderVector.insert(it, bn);
                        break;
                    }   
                }
            }else{
                bullDonators.insert(dn);
                orderVector.push_back(bn);
            }
       }
        
    }
    fin.close();

    //read from female input file
    fin.open(femaleInput);
    currentFile = femaleInput;
    for (int i=1; getline(fin, line); i++) {
        string trimmedLine = trimString(line);
        //skip empty rows and headers
        if(trimmedLine != "" && trimmedLine != "femalenamemembernamememberemailrankbull1bull2bull3bull4bull5bull6"){
            stringstream ss(line);
            string fn, mn, me, rankstr, bull;
            stringstream intstream;
            int r;
            list<Bull*> bullsList;

            //separate first four fields
            getline(ss, fn, ',');
            getline(ss, mn, ',');
            getline(ss, me, ',');
            getline(ss, rankstr, ',');
            r = isIntegerCheck(rankstr, currentFile, i);

            //read into bull list
            while (getline(ss, bull, ',')) { 
                if (bullMap.count(bull)){
                    bullsList.push_back(&bullMap.at(bull));
                }
                else if(bull != ""){
                    cout << "Warning: bull with name " << bull << " could not be found." << endl;
                }
            }

            // insert female record into vector next to female cows with the same junior member
            Female newFemale(fn,mn,me,r,bullsList);
            if(femaleMembers.count(mn)){
                vector<Female>::iterator it = femaleVector.end();
                while(it != femaleVector.begin()){
                    it--;
                    if(mn == it->memberName){
                        it++;
                        femaleVector.insert(it, newFemale);
                        break;
                    }   
                }
            }else{
                femaleMembers.insert(mn);
                femaleVector.push_back(newFemale);
            }
       }
    }
    fin.close();

    //create 2D vector of indexes at their corresponding rank
    //eg rankIndexes[0] will contain a vector with the indexes in femaleVector of every female cow with rank 1
    // rankIndexes[1] will contain all indexes for rank 2 and so on
    for(int i=0; i < femaleVector.size(); i++){
        int r = femaleVector[i].rank;
        while(rankIndexes.size() < r){
            vector<int> v;
            rankIndexes.push_back(v);
        }
        rankIndexes[r-1].push_back(i);
    }

    //the actual draw part of the program:
    //starting with n=1, a cow with rank n is randomly drawn from the pool of cows with rank n
    //starting with the first bull in this cow's ranked list of bulls, 
    //if the bull has two or more straws left to donate, remove two straws from the bull's straw count, and match the bull with the cow
    //if the bull has less then two straws, check all the remaining bulls in order until a match is found, or the end of the list of bulls is reached
    //repeat this process for all cows in all ranks

    //get vector of all indexes of cows with rank n
    for(vector<int> currentRank: rankIndexes){
        //randomize the order of indexes within this rank
        random_shuffle(currentRank.begin(), currentRank.end());
        //get random index from rank n
        for(int currentIndex: currentRank){
            //get Bull from bull list for cow at specified index
            for(Bull* currentBull: femaleVector[currentIndex].bullChoices){
                //check straw count
                if (currentBull->straws >= 2){
                    currentBull->straws -= 2;
                    //match current cow with current bull
                    femaleVector[currentIndex].bullMatch = currentBull;
                    //add junior member to list
                    currentBull->juniors.push_back(femaleVector[currentIndex].memberName);
                    cout << femaleVector[currentIndex].femaleName << " matched with " << currentBull->bullName << "\n";
                    break;
                }
            }
        }
    }


    ofstream outFile = createReplaceFile("donator_output.csv");
    string currentDonator = "";

    //create donator output
    //each donator that donated bulls for the draw will receive a table with an entry for each junior member that received semen from one or more of their bulls, along with:
    //the name of the bull, and number of straws received
    //as well as an entry for bulls that didn't get drawn by any juniors
    for(string someBullName: orderVector){
        Bull someRecord = bullMap.at(someBullName);

        //generate result header for new donator
        if(currentDonator != someRecord.donatorName){
            currentDonator = someRecord.donatorName;
            outFile << ",," << endl;
            outFile << "Result for," << someRecord.donatorName << ",at " << someRecord.donatorEmail << endl;
            outFile << "Junior Name,Bull Name,Number of Straws" << endl;
        }
        someRecord.juniors.sort();
        string currentJunior = "";
        int juniorCount = 0;
        //Aggregate how many times current junior appears in this list
        for(string junior: someRecord.juniors){
            if(currentJunior == ""){
                currentJunior = junior;
                juniorCount++;
            }
            else if(currentJunior == junior){
                juniorCount++;
            }
            else{
                outFile << currentJunior << "," << someBullName << "," << juniorCount*2 << endl;
                currentJunior = junior;
                juniorCount = 1;
            }
        }
        //handles case where juniorCount==0
        outFile << currentJunior << "," << someBullName << "," << juniorCount*2 << endl;
       
    }
    outFile.close();

    outFile = createReplaceFile("junior_output.csv");
    string currentJunior = "";

    //create junior output
    //each junior member that participated in the draw will receive a table with an entry for each cow they entered in the draw, along with:
    //name of bull that cow will semen from (if any), number of straws (if any), and email of donator to contact (if any)
    for(Female someFemale: femaleVector){

        //generate result header for new junior member
        if(currentJunior != someFemale.memberName){
            currentJunior = someFemale.memberName;
            outFile << ",,," << endl;
            outFile << "Result for," << someFemale.memberName << ",at," << someFemale.memberEmail << endl;
            outFile << "Female Name,Bull Name,Number of Straws,Email" << endl;
        }

        if(someFemale.bullMatch != nullptr){
            //output if match was found
            outFile << someFemale.femaleName << "," << someFemale.bullMatch->bullName << ",2," << someFemale.bullMatch->donatorEmail << endl;
        }
        else{
            //output if match was not found
            outFile << someFemale.femaleName << ",None,," << endl;
        }
    }
    outFile.close();

    cout << "draw complete, see donator_output and junior_output for results" << endl;

    return(0);
}