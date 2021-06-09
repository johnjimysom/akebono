// akebono.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream

//using namespace std;


/*****************************************************************************************
* read_csv
*
* reading the .csv file, can start a translation of hex.
*****************************************************************************************/

std::vector<std::pair<std::string, std::vector<int>>> read_csv(std::string filename) {
    // Reads a CSV file into a vector of <string, vector<int>> pairs where
    // each pair represents <column name, column values>

    // Create a vector of <string, int vector> pairs to store the result
    std::vector<std::pair<std::string, std::vector<int>>> result;

    // Create an input filestream
    std::ifstream myFile(filename);

    // Make sure the file is open
    if (!myFile.is_open()) throw std::runtime_error("Could not open file");

    // Helper vars
    std::string line, colname;
    int val;

    // Read the column names
    if (myFile.good())
    {
        // Extract the first line in the file
        std::getline(myFile, line);

        // Create a stringstream from line
        std::stringstream ss(line);

        // Extract each column name
        while (std::getline(ss, colname, ',')) {

            // Initialize and add <colname, int vector> pairs to result
            result.push_back({ colname, std::vector<int> {} });
        }
    }

    // Read data, line by line
    while (std::getline(myFile, line))
    {
        // Create a stringstream of the current line
        std::stringstream ss(line);

        // Keep track of the current column index
        int colIdx = 0;

        // Extract each integer
        while (ss >> val) {

            // Add the current integer to the 'colIdx' column's values vector
            result.at(colIdx).second.push_back(val);

            // If the next token is a comma, ignore it and move on
            if (ss.peek() == ',') ss.ignore();

            // Increment the column index
            colIdx++;
        }
    }

    // Close file
    myFile.close();

    return result;
} // END std::vector<std::pair<std::string, std::vector<int>>> read_csv(std::string filename)

/*****************************************************************************************
* void write_csv
*
* note: we can translate the hexicmal to binary and have it parse here
*****************************************************************************************/

void write_csv(std::string filename, std::string colname, std::vector<int> vals) {
    // Make a CSV file with one column of integer values
    // filename - the name of the file
    // colname - the name of the one and only column
    // vals - an integer vector of values

    // Create an output filestream object
    std::ofstream myFile(filename);

    // Send the column name to the stream
    myFile << colname << "\n";

    // Send data to the stream
    for (int i = 0; i < vals.size(); ++i)
    {
        myFile << vals.at(i) << "\n";
    }

    // Close the file
    myFile.close();
}// END void write_csv(std::string filename, std::string colname, std::vector<int> vals)


/*****************************************************************************************
* void write_csv 
*
* note: we can translate the hexicmal to binary and have it parse here on multi vectors?
*****************************************************************************************/
void write_csv(std::string filename, std::vector<std::pair<std::string, std::vector<int>>> dataset) {
    // Make a CSV file with one or more columns of integer values
    // Each column of data is represented by the pair <column name, column data>
    //   as std::pair<std::string, std::vector<int>>
    // The dataset is represented as a vector of these columns
    // Note that all columns should be the same size

    // Create an output filestream object
    std::ofstream myFile(filename);

    // Send column names to the stream
    for (int j = 0; j < dataset.size(); ++j)
    {
        myFile << dataset.at(j).first;
        if (j != dataset.size() - 1) myFile << ","; // No comma at end of line
    }
    myFile << "\n";

    // Send data to the stream
    for (int i = 0; i < dataset.at(0).second.size(); ++i)
    {
        for (int j = 0; j < dataset.size(); ++j)
        {
            myFile << dataset.at(j).second.at(i);
            if (j != dataset.size() - 1) myFile << ","; // No comma at end of line
        }
        myFile << "\n";
    }

    // Close the file
    myFile.close();
}


int main() {
   
    // [***one****]
    // Make a vector of length 10 filled with 99s
    std::vector<int> vec(20, 99);

    // [***one****]
    // Write the vector to CSV
    write_csv("qr-scan-sample_binary.csv", "binary", vec);

    // [***1,2,3****]
    // Make three vectors, translated
    std::vector<int> vec1(5, 1); //write the SK item number
    std::vector<int> vec2(5, 2); //write the stock
    std::vector<int> vec3(5, 3); //write the item number (global serial)

    // [***1,2,3****]
    // Wrap into a vector
    std::vector<std::pair<std::string, std::vector<int>>> vals = { {"One", vec1}, {"Two", vec2}, {"Three", vec3} };

    // [***1,2,3****]
    // Write the vector to .csv file
    write_csv("three_vector.csv", vals); //debug

    //[READ .csv]
    // Read three_cols.csv and ones.csv
    std::vector<std::pair<std::string, std::vector<int>>> three_cols = read_csv("three_vector.csv"); //three vectors
    std::vector<std::pair<std::string, std::vector<int>>> ones = read_csv("qr-scan-sample.csv"); //just print binary

    // Write to another file to check that this was successful
    write_csv("three_vector_copy2.csv", three_cols); //debug but didn't parse numbers
    //write_csv("qr-scan-sample_binary_copy.csv", ones); //throws out of memory if kept same file name.

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
