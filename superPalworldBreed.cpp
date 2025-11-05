#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "json.hpp"

using json = nlohmann::json;


struct Pal {
  std::string id;
  std::string key;
  std::string name;

  Pal() {}

  Pal(std::string _id, std::string _key, std::string _name)
      : id(_id), key(_key), name(_name) {}
};


struct Breed {
  std::string parent1_key;
  std::string parent2_key;
  std::string child_key;

  Breed() {}

  Breed(std::string _parent1_key, std::string _parent2_key, std::string _child_key)
      : parent1_key(_parent1_key), parent2_key(_parent2_key), child_key(_child_key) {}
};


std::vector<Pal> getPalList(std::string fileName) {
  std::ifstream file_pals(fileName);
  if (!file_pals.is_open()) {
    std::cerr << "Error opening JSON files: " << fileName << std::endl;
    return {};
  }
  json pals = json::parse(file_pals);
  std::vector<Pal> palList;

  for (const auto & pal : pals) {
    Pal newPal(
        pal["id"].get<std::string>(),
        pal["key"].get<std::string>(),
        pal["name"].get<std::string>());
    palList.push_back(newPal); 
  }
  return palList;
}


std::vector<Breed> getBreedList(std::string fileName) {
  std::ifstream file_breeds(fileName);
  if (!file_breeds.is_open()) {
    std::cerr << "Error opening JSON files: " << fileName << std::endl;
    return {};
  }
  json breeds = json::parse(file_breeds);
  std::vector<Breed> breedList;

  for (const auto & breed : breeds) {
    Breed newBreed(
        breed["parent1"].get<std::string>(),
        breed["parent2"].get<std::string>(),
        breed["child"].get<std::string>());
    breedList.push_back(newBreed);
  }
  return breedList;
}


int main(int argc, char ** argv) {
  auto pals = getPalList("data/pals.json");
  auto breeds = getBreedList("data/breed.json");
  std::cout << pals.size() << " " << breeds.size() << std::endl;
  return 0;
}
