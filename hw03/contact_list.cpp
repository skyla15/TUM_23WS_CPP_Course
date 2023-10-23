#include "contact_list.h"

#include <algorithm>
#include <numeric>
#include <sstream>
#include <iostream> 


#include <iomanip> // setw

// TODO create implementation here!
namespace contact_list {

// Add a contact. Disallow empty or duplicate names and return false. return true for success.
bool add(storage& contacts, std::string_view name, number_t number){

    if (get_number_by_name(contacts, name) == -1) {
        if(!name.empty()){
            contacts.names.push_back(std::string{name});
            contacts.numbers.push_back(number);
            return true;
        }
    }
    return false;
}


// A function that returns how many contacts are currently stored
size_t size(const storage& contacts){
    return contacts.names.size();
}


// Get the number for a given name - return -1 if no such name is found.
number_t get_number_by_name(storage& contacts, std::string_view name){
    
    for (size_t i = 0; i < contacts.names.size(); ++i) {
        if (contacts.names.at(i) == name) {
            return contacts.numbers.at(i);
        } 
    }
    return -1;
}


std::string to_string(const storage& contacts){


    // auto new_str = std::string(n_zero - std::min(n_zero, old_str.length()), '0') + old_str;

    std::ostringstream contact_list;
    // get the longest name length
    int max_length;
    for( size_t i = 0 ; i < contacts.names.size() ; ++i){
        max_length = std::max(max_length, static_cast<int>(contacts.names[i].length()));
    }

    for( size_t i = 0 ; i < contacts.names.size() ; ++i){
        const auto& name = contacts.names.at(i);
        // #include <iomanip>
        contact_list << std::left << std::setw(max_length) << name << " - " \
                  << contacts.numbers[i] << "\n";  
    }

    return contact_list.str();
}


// Remove a contact. 
// Does nothing and return false if requested name was not part of the list. 
// return true for success.
bool remove(storage& contacts, std::string_view name){
    
    for ( size_t i = 0 ; i < contacts.names.size() ; ++i){
        if ( contacts.names.at(i) == name ){
            contacts.names.erase(contacts.names.begin() + i);
            contacts.numbers.erase(contacts.numbers.begin() + i);   
            return true;
        }
    }
    return false;
}


// Sort the contact list by name - watch out to keep the number list synchronized!
void sort(storage& contacts){

    // if(contacts.names.size() <= 1){
    //     return;
    // }

    // std::string contact_list{to_string(contacts)}; 
    // std::stringstream ss(contact_list);
    // std::string line;
    // std::vector<std::string> lines;
    // // split lines 
    // while(std::getline(ss, line, '\n' )){
    //     lines.push_back(line);
    // }

    // std::vector<std::string> sorted_names;
    // std::vector<number_t> sorted_numbers;

    // sort(lines.begin(), lines.end());
    // for(size_t i = 0 ; i < lines.size() ; ++i){
    //     for(size_t j = 0 ; j < lines[i].length() ; ++j){
    //         if(lines[i][j] == '-'){
    //             std::string name{lines[i].substr(0, j)};
    //             number_t number{std::stoi((lines[i].substr(j+1, lines[i].length())))};
    //             sorted_names.push_back(name);
    //             sorted_numbers.push_back(number);
    //         }
    //     }
    // }

    // contacts.numbers = sorted_numbers;
    // contacts.names = sorted_names;

////////////////////////////////////////////////////

    // make a new list of indices 
    // https://stackoverflow.com/questions/37368787/c-sort-one-vector-based-on-another-one
    std::vector<int> indices(contacts.names.size());
    // make a list in increasing order from 0 within the address ranges of indices.
    std::iota(indices.begin(), indices.end(), 0);
    // sort indices in the order of names to use it for the number.
    std::sort(
            std::begin(indices),
            std::end(indices),
            [&contacts](const size_t& A, const size_t& B){
                return contacts.names[A] < contacts.names[B];
                }
            );
    
    std::vector<std::string> sorted_names;
    sorted_names.reserve(contacts.names.size());
    std::vector<number_t> sorted_numbers;
    sorted_numbers.reserve(contacts.numbers.size());

    // sort the number and name 
    for( size_t index : indices ){
        sorted_names.push_back(contacts.names[index]);
        sorted_numbers.push_back(contacts.numbers[index]);
    }

    contacts.numbers = sorted_numbers;
    contacts.names = sorted_names;
}

std::string get_name_by_number(storage& contacts, number_t number){
    for( size_t i = 0 ; i < contacts.numbers.size() ; ++i){
        if ( contacts.numbers.at(i) == number ){
            return contacts.names.at(i);
        } 
    }
    return std::string("");
}

}