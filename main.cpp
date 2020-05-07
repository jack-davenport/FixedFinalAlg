#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <ctime>
#include <math.h>
using namespace std;

vector<int> trivial(string text, string pattern);//pass in the text and looks for every location of pattern using trivial search
vector<int> rabinKarpSearch(string text, string pattern, int primeNumber);//uses rabin karp taking in prime nunmber text and pattern to look for every pattern matchw
void outputToFile(fstream& output, vector<int>& rkVector, vector<int>& trivialVector, clock_t rkTime, clock_t trivialTime, bool showPatternLocation, string inputFile, string pattern);
void readControlFile(fstream& ctrl, string out);

int main(int argc, char* argv[])
{
//    if(argc < 2)//if not enough arguments passed in
//    {
//        cout << "Not enough arguments provided" << endl;
//        return -1;
//    }

    fstream ctrl;//file that you enter the text files u want to test
    ctrl.open(argv[1], ios::in);//open as input
    if(!ctrl)//check if opened
    {
        cout << "Couldn't open control" << endl;
        return -1;
    }
    readControlFile(ctrl, "output.txt");//call function to read through the file

    ctrl.close();
    cout << "i reach end" << endl;
    return 0;
}



vector<int> trivial(string text, string pattern)
{
    vector<int> indexOfMatches;//return vector
    int patLength = pattern.length(); //variable for pattern length
    for(int i= 0; i < text.length() - patLength + 1; i++)//iterates through the entire text - pattern length + 1
    {
        if(text.substr(i, patLength) == pattern)//compares text section to pattern
        {
            indexOfMatches.push_back(i); // adds to indexofmatches
        }
    }

    return indexOfMatches;//returns the indices
}

vector<int> rabinKarpSearch(string text, string pattern, int primeNumber)
{
    vector<int> patternMatches;//vector holding indices of the pattern matches
    int chars = 256;//number of characters in the alphabet
    int pHashVal = 0;//pattern hash value
    int subStringHashVal = 0;//substring hash value initialize to 0
    int hash = 1;

    for(int i = 0; i < pattern.length(); i++)
    {
        pHashVal = (chars * pHashVal + pattern.at(i)) % primeNumber;//determing pattern hash val
        subStringHashVal = (chars * subStringHashVal +  text.at(i)) % primeNumber; //d/etermine prime read hash val
        if(i < pattern.length() - 1)
            hash = (chars * hash) % primeNumber;//hash = chars * hash % primenumber does a for loop instead of power function bc that will lead to a overflow
    }


    for(int i = 0; i <= text.length() - pattern.length(); i++)//iterate through length of text - length of pattern because rolling hash we dont iterate through each
    {
        if(pHashVal == subStringHashVal)//check if rolling hash value ='s the pattern hash value
        {
            if (text.substr(i, pattern.length()) == pattern)//can have collisions in hash so have to check two strings
            {
                patternMatches.push_back(i);//adds i to patternMAtches the vector i will return
            }
        }

        if(i < text.length() - pattern.length())
        {
            //subtract the value of text[i] - hash creates a rolling hash
            subStringHashVal = ((chars * (subStringHashVal - text.at(i) * hash) + text.at(i + pattern.length()) )% primeNumber);

            if(subStringHashVal < 0)//takes care of cases where substring hash is negative
            {
                subStringHashVal += primeNumber;
            }
        }
    }

    return patternMatches;
}

void readControlFile(fstream& ctrl, string out)//takes in fstream control file
{
    fstream output;//declares output.txt
    output.open(out, ios::out);//opens output file
    if(!output)//if cant open the output file
    {
        cout << "couldn't open" <<endl;
        throw "Couldn't open output";
    }

    int counter = 0;//counter for while loop instead of eof
    int numberOfInputFiles;//declares int to be read in
    ctrl >> numberOfInputFiles;//reads in first val from control file
    ctrl.ignore(60,'\n');//skips over \r that was appearing

    string inputFileName;//declares string to be used as name of input files
    getline(ctrl, inputFileName);//extract string from ctrl
    inputFileName.erase(std::remove(inputFileName.begin(), inputFileName.end(), '\r'));//removes \r from it

    string pattern;//string for the pattern
    getline(ctrl, pattern);//reads in pattern from ctrl
    pattern.erase(std::remove(pattern.begin(), pattern.end(), '\r'));//removes \r

    int showLocationsOfMatches;//int that used as a bool on whether to show all locations of matches to pattern
    ctrl >> showLocationsOfMatches;//reads in int

    ifstream ifs(inputFileName);//if stream to extract data from text
    string text;//declares string text
    text.assign( (std::istreambuf_iterator<char>(ifs) ),
                 (std::istreambuf_iterator<char>()    ) );//puts the read in textfile into variable

    //all these are before the while loop to do a prime read to fix errors that can happen
    while(counter < numberOfInputFiles)
    {
        counter++;//increments counter

        std::clock_t startTrivial;
        startTrivial = std::clock();//used to time the trivial function
        vector<int> trivialVector= trivial(text, pattern);//runs the trivial function and saves the returned vector locally in triv
        startTrivial = std:: clock() - startTrivial;//calculates time of the function

        std::clock_t rkTime;//used to time the rabin-karp function
        rkTime = std::clock();
        vector<int> rkVector = rabinKarpSearch(text, pattern, 263);//runs the rabinKarpsearch and saves the returned vector locally in rkVector
        rkTime = std::clock() - rkTime;//gets the time of the functio

        //to show indices or not change last val to true to show or false to not show
        outputToFile(output, rkVector, trivialVector, rkTime, startTrivial, showLocationsOfMatches, inputFileName, pattern);

        if(counter != numberOfInputFiles) //added because ran into problems where it was trying to read an empty file and crashing
        {
            ctrl.ignore(60,'\n'); //ignores \r thats left over from prime read and any subsequent reads
            getline(ctrl, inputFileName);//gets new input file name
            inputFileName.erase(std::remove(inputFileName.begin(), inputFileName.end(), '\r'));//gets rid of \r

            getline(ctrl, pattern);//gets new pattern from ctrl
            pattern.erase(std::remove(pattern.begin(), pattern.end(), '\r'));//takes pattern and removes the /r

            ctrl >> showLocationsOfMatches;//reads in bool
            ifs.close();//closes file
            ifs.clear();//allows ifs to be reassigned to new file
            ifs.open(inputFileName, ios::in);//opens new file
            text.assign( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );//puts the read in textfile into variable
        }
        else
        {
            ifs.close();
        }

    }

    output.close();
}

void outputToFile(fstream& output, vector<int>& rkVector, vector<int>& trivialVector, clock_t rkTime, clock_t trivialTime, bool showPatternLocation, string inputFile, string pattern)
{
    output << endl << "Input File: " << inputFile  << " Searching for: " << pattern << endl;
    output << endl <<"Trivial" << endl;//labels this section as trivial info
    output << "Trivial Time: " <<trivialTime / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;//adds the time of function to output
    output << "Instances of pattern found: " << trivialVector.size() << endl; //how many pattern matches in text

    if(showPatternLocation)//if wanted to show the indices
    {
        for(int i = 0; i < trivialVector.size() - 1; i++)//prints the index the text matches the pattern at for triival
        {
            output << trivialVector.at(i) << ", ";
            if(i % 20 == 0 && i != 0)
                output << endl;
        }
        output << trivialVector.at(trivialVector.size() - 1) << endl;
    }


    output << endl << "Rabin-Karp" << endl;//labels this section as rabin karp
    output << "Rabin-Karp Time: " << rkTime / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;//adds the timing of rk function to the output
    output << "Instances of pattern found: " << rkVector.size() << endl;//adds how many pattern matches in the text were found

    if(showPatternLocation)//shows indices or not depending on bool
    {
        for(int i = 0; i < rkVector.size() - 1; i++)//prints the index the text matches the pattern at for rabin-karp
        {
            output << rkVector.at(i) << ", ";
            if(i % 20 == 0 && i != 0)
                output << endl;
        }
        output << rkVector.at(rkVector.size() - 1) << endl;
    }
}



