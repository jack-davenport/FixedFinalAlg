#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <fstream>
#include <ctime>
#include <math.h>
using namespace std;

vector<int> trivial(string text, string pattern);
vector<int> rabinKarpSearch(string text, string pattern, int primeNumber);
void outputToFile(fstream& output, vector<int> RK, vector<int> triv);

int main(int argc, char* argv[])
{
    ifstream ifs(argv[1]);
    string text( (std::istreambuf_iterator<char>(ifs) ),
                 (std::istreambuf_iterator<char>()    ) );//puts the read in textfile into variable

    std::clock_t start;

    start = std::clock();
    vector<int> triv = trivial(text, "the");

    std::cout << "Trivial Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;


    start = std::clock();
    vector<int> RK = rabinKarpSearch(text, "the", 101);
    std::cout << "Rabin-Karp Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

    cout << "Trivial: " << triv.size() << endl;
    cout << "Rabin-Karp: " << RK.size() << endl;

    fstream output;
    output.open(argv[2], ios::out);
    if(!output)
    {
        cout << "Didn't open output file" << endl;
        return -1;
    }

    outputToFile(output, RK, triv);
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

    return indexOfMatches;
}

vector<int> rabinKarpSearch(string text, string pattern, int primeNumber)
{
    vector<int> patternMatches;//vector holding indices of the pattern matches
    int chars = 256;//number of characters in the alphabet
    int pHashVal = 0;//pattern hash value
    int subStringHashVal = 0;
    int hash = 1;

    hash = static_cast<int>(pow(chars, pattern.length() - 1)) % primeNumber; //gets a hash that is valid based on the pattern

    for(int i = 0; i < pattern.length(); i++)
    {
        pHashVal = (chars * pHashVal + pattern.at(i)) % primeNumber;//determing pattern hash val
        subStringHashVal = (chars * subStringHashVal +  text.at(i)) % primeNumber; //d/etermine prime read hash val
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

void outputToFile(fstream& output, vector<int> RK, vector<int> triv)
{
    output << "Trivial" << endl;
    for(int i = 0; i < triv.size() - 1; i++)
    {
        output << triv.at(i) << ", ";
        if(i % 20 == 0 && i != 0)
            output << endl;
    }
    output << triv.at(triv.size() - 1) << endl;

    output << "Rabin Karp" << endl;
    for(int i = 0; i < RK.size() - 1; i++)
    {
        output << RK.at(i) << ", ";
        if(i % 20 == 0 && i != 0)
            output << endl;
    }
    output << RK.at(RK.size() - 1) << endl;

    output.close();
}
