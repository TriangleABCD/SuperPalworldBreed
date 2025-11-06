#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>

#include "json.hpp"

using json = nlohmann::json;


struct Pal {
  int id;
  std::string key;
  std::string name;

  static std::map<int, std::string> id2key;
  static std::map<std::string, int> key2id;

  static std::map<int, std::string> id2name;
  static std::map<std::string, int> name2id;

  Pal() {}

  Pal(int _id, std::string _key, std::string _name)
      : id(_id), key(_key), name(_name) {}
};

std::map<int, std::string> Pal::id2key;
std::map<std::string, int> Pal::key2id;
std::map<int, std::string> Pal::id2name;
std::map<std::string, int> Pal::name2id;

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
        -1,
        pal["key"].get<std::string>(),
        pal["name"].get<std::string>());
    palList.push_back(newPal); 
  }
  for (size_t i = 0; i < palList.size(); i++) {
    palList[i].id = i;
    Pal::id2key[i] = palList[i].key;
    Pal::key2id[palList[i].key] = i;
    Pal::id2name[i] = palList[i].name;
    Pal::name2id[palList[i].name] = i;
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


std::vector<std::vector<int>> getMatrix(const std::vector<Pal> & palList, const std::vector<Breed> & breedList) {
  int n = palList.size();
  std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));
  for (const auto & breed : breedList) {
    int parent1_id = Pal::key2id[breed.parent1_key];
    int child_id = Pal::key2id[breed.child_key];
    matrix[parent1_id][child_id]++;
  }
  return matrix;
}


std::vector<Breed> getBreeds(const std::vector<Breed> & breeds, int parent1_id, int child_id) {
  std::vector<Breed> result;
  std::string parent1_key = Pal::id2key[parent1_id];
  std::string child_key = Pal::id2key[child_id];

  for (const auto & breed : breeds) {
    if (breed.parent1_key == parent1_key && breed.child_key == child_key) {
      result.push_back(breed);
    }
  }
  return result;
}


void printBreedPath(const std::vector<Breed> & breeds, 
                    const std::map<int, int> & father, int start_id, int end_id) {

  std::cout << Pal::id2name[start_id] << " -> " << Pal::id2name[end_id] << " 的配种路径：" << std::endl;
  std::cout << "###########################################" << std::endl;

  std::stack<int> path;
  int cur = end_id;
  while (cur != start_id) {
    path.push(cur);
    cur = father.at(cur);
  }
  path.push(start_id);

  int idx = 1;

  while (path.size() > 1) {
    int parent1_id = path.top();
    path.pop();
    int child_id = path.top();

    auto breedOptions = getBreeds(breeds, parent1_id, child_id);
    if (breedOptions.empty()) {
      std::cout << "No breeding options from " << Pal::id2name[parent1_id]
                << " to " << Pal::id2name[child_id] << std::endl;
      return;
    }
    
    std::cout << "Step " << idx << ": " << Pal::id2name[parent1_id] << " x " 
              << "??? -> " << Pal::id2name[child_id] << std::endl;
    for (const auto & breed : breedOptions) {
      std::string parent2_name = Pal::id2name[Pal::key2id[breed.parent2_key]];
      std::cout << "    Option: " << Pal::id2name[parent1_id] << " x " 
                << parent2_name << " -> " << Pal::id2name[child_id] << std::endl;
    }
    std::cout << "###########################################" << std::endl;
    idx++;
  }
}


int main(int argc, char ** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <pals.json> <breed.json>" << std::endl;
    return 1;
  }
  std::string palsFile = argv[1];
  std::string breedFile = argv[2];

  auto pals = getPalList(palsFile);
  auto breeds = getBreedList(breedFile);
  auto matrix = getMatrix(pals, breeds);

  std::string pal1_name, pal2_name;
  std::cout << "请输入两个想要查询配种路径的帕鲁名称，用空格分隔：" << std::endl;
  std::cin >> pal1_name >> pal2_name;
  int pal1_id = Pal::name2id[pal1_name];
  int pal2_id = Pal::name2id[pal2_name];


  std::queue<int> que;
  que.push(pal1_id);
  std::map<int, bool> visited;
  std::map<int, int> father;
  visited[pal1_id] = true;
  
  bool found = false;
  while (!que.empty()) {
    auto cur = que.front();
    que.pop();
    if (cur == pal2_id) {
      found = true;
      break;
    }

    for (size_t i = 0; i < pals.size(); i++) {
      if (visited[pals[i].id]) continue;
      if (matrix[cur][pals[i].id] > 0) {
        que.push(pals[i].id);
        visited[pals[i].id] = true;
        father[pals[i].id] = cur;
      }
    }
  }

  if (!found) {
    std::cout << pal1_name << " x " << pal2_name << " 不存在配种路径" << std::endl;
  }

  printBreedPath(breeds, father, pal1_id, pal2_id); 
  return 0;
}
