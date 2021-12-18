#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Node {
  long long value = -1;
  int left_v = -1;
  int right_v = -1;
  Node* left = nullptr;
  Node* right = nullptr;

  friend std::ostream& operator << (std::ostream& os, const Node& n);
};

std::ostream& operator << (std::ostream& os, const Node& n) {
  os << n.left_v << ' ' << n.right_v << '\n';
  return os;
}

Node* convert_to_tree(const std::string& s, int& index) {
  auto n = new Node();
  if (s[index] == '[') {
    if (s[index + 1] == '[') {
      index += 1;
      n->left = convert_to_tree(s, index);
    } else {
      const auto index2 = s.find(",", index + 1);
      n->left_v = std::stoi(s.substr(index+1, index2 - index-1));
      index = index2;
    }
  }

  if (s[index] == ',') {
    if (s[index + 1] == '[') {
      index += 1;
      n->right = convert_to_tree(s, index);
    } else {
      const auto index2 = s.find("]", index);
      n->right_v = std::stoi(s.substr(index+1, index2 - index-1));
      index = index2;
    }
  }
  index++;
  return n;
}

void traverse(Node* n) {
  std::cout << "[";
  if (n->left_v != -1) {
    std::cout << n->left_v;
  } else {
    traverse(n->left);
  }
  std::cout << ',';
  if (n->right_v != -1) {
    std::cout << n->right_v;
  } else {
    traverse(n->right);
  }
  std::cout << "]";
}

void traverse_reverse(Node* n) {
  std::cout << "[";
  if (n->right_v != -1) {
    std::cout << n->right_v;
  } else {
    traverse_reverse(n->right);
  }
  std::cout << ',';
  if (n->left_v != -1) {
    std::cout << n->left_v;
  } else {
    traverse_reverse(n->left);
  }
  std::cout << "]";
}

void set_value_to_n_one(Node* n) {
  if (n->left_v == -1) {
    set_value_to_n_one(n->left);
  }
  n->value = -1;
  if (n->right_v == -1) {
    set_value_to_n_one(n->right);
  }
}

Node* check_explode(Node* n, int level) {
  if (level >= 4) {
    if (n->left == nullptr && n->right == nullptr) {
      return n;
    }
  }
  if (n->left != nullptr) {
    auto returned_node = check_explode(n->left, level + 1);
    if (returned_node != nullptr) {
      return returned_node;
    }
  }
  if (n->right != nullptr) {
    auto returned_node = check_explode(n->right, level + 1);
    if (returned_node != nullptr) {
      return returned_node;
    }
  }
  return nullptr;
}

std::pair<bool, Node*> traverse_update_expoded_l_r(Node* n, Node* exploded, Node*& prev) {
  if (n->left_v != -1) {
    prev = n;
  } else {
    if (n->left == exploded) {
      if (prev!=nullptr && prev->right == nullptr) {
        prev->right_v += exploded->left_v;
      } else if (prev!=nullptr && prev->left == nullptr){
        prev->left_v += exploded->left_v;
      }
      return {true, n};
    } else {
      auto [found, node] = traverse_update_expoded_l_r(n->left, exploded, prev);
      if (found) return {true, node};
    }
  }
  if (n->right_v != -1) {
    prev = n;
  } else {
    if (n->right == exploded) {
      if (prev != nullptr && prev->right == nullptr) {
        prev->right_v += exploded->left_v;
      } else if (prev != nullptr && prev->left == nullptr){
        prev->left_v += exploded->left_v;
      }
      return {true, n};
    } else {
      auto [found, node] = traverse_update_expoded_l_r(n->right, exploded, prev);
      if (found) return {true, node};
    }
  }
  return {false, nullptr};
}

std::pair<bool, Node*> traverse_update_expoded_r_l(Node* n, Node* exploded,  Node*&prev) {
  if (n->right_v != -1) {
    prev = n;
  } else {
    if (n->right == exploded) {
      if (prev != nullptr && prev->left == nullptr) {
        prev->left_v += exploded->right_v;
      } else if (prev != nullptr && prev->right == nullptr) {
        prev->right_v += exploded->right_v;
      }
      return {true, n};
    } else {
      auto [found, node] = traverse_update_expoded_r_l(n->right, exploded, prev);
      if (found) return {true, node};

    }
  }
  if (n->left_v != -1) {
    prev = n;
  } else {
    if (n->left == exploded) {
      if (prev != nullptr && prev->left == nullptr) {
        prev->left_v += exploded->right_v;
      } else if (prev != nullptr && prev->right == nullptr) {
        prev->right_v += exploded->right_v;
      }
      return {true, n};
    } else {
      auto [found, node] = traverse_update_expoded_r_l(n->left, exploded, prev);
      if (found) return {true, node};
    }
  }
  return {false, nullptr};

}

bool explode(Node* root) {
  auto prev = root;
  auto index = 0;
  auto exploded_node = check_explode(root, index);
  if (exploded_node!= nullptr) {
    prev = nullptr;
    auto [found_l, to_update_l] = traverse_update_expoded_l_r(root, exploded_node, prev);
    prev = nullptr;
    auto [found_r, to_update_r] = traverse_update_expoded_r_l(root, exploded_node, prev);
    if (to_update_l != nullptr && to_update_l->left == exploded_node) {
      to_update_l->left = nullptr;
      to_update_l->left_v = 0;
    }
    if (to_update_r != nullptr && to_update_r->right == exploded_node) {
      to_update_r->right = nullptr;
      to_update_r->right_v = 0;
    }
    delete exploded_node;
  }
  return (exploded_node != nullptr);
}

bool split(Node* n) {
  if (n->left == nullptr) {
    if (n->left_v >= 10) {
      n->left = new Node();
      n->left->left_v = n->left_v / 2;
      n->left->right_v = n->left_v - n->left->left_v;
      n->left_v = -1;
      return true;
    }
  } else {
    auto found = split (n->left);
    if (found) return true;
  }
  if (n->right == nullptr) {
    if (n->right_v >= 10) {
      n->right = new Node();
      n->right->left_v = n->right_v / 2;
      n->right->right_v = n->right_v - n->right->left_v;
      n->right_v = -1;
      return true;
    }
  } else {
    auto found = split (n->right);
    if (found) return true;
  }
  return false;
}

int find_approrpiate_action(Node* n, int level) {
  if (level >= 4 && n->left == nullptr && n->right == nullptr) {
    return 1; // explodable
  }
  if (n->left_v != -1) {
    if(n->left_v >= 10) {
      return 2; // splittable
    }
  } else {
    auto ans = find_approrpiate_action(n->left, level + 1);
    if (ans != 0) {
      return ans;
    }
  }
  if (n->right_v != -1) {
    if(n->right_v >= 10) {
      return 2; // splittable
    }
  } else {
    auto ans = find_approrpiate_action(n->right, level + 1);
    if (ans != 0) {
      return ans;
    }
  }
  return 0;
}

void sum_nodes(Node *n ) {
  n->value = 0;

  if (n->left != nullptr) {
    if (n->left->value == -1) sum_nodes(n->left);
    n->value += 3 * n->left->value;
  } else {
    n->value += 3 * n->left_v;
  }

  if (n->right != nullptr) {
    if (n->right->value == -1) sum_nodes(n->right);
    n->value += 2 * n->right->value;
  } else {
    n->value += 2 * n->right_v;
  }
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_18_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::getline(file, line);
  auto index = 0;
  auto prev = convert_to_tree(line, index);
  auto root = prev;
  while(std::getline(file, line))  {
    Node* n = new Node();
    n->left = prev;
    index = 0;
    n->right = convert_to_tree(line, index);
    prev = n;

    // traverse(n);
    // std::cout << '\n';

    while(true) {
      if (explode(n)) continue;
      if (split(n)) continue;
      break;
    }

    // traverse(n);
    // std::cout << '\n';
    root = n;
  }

  set_value_to_n_one(root);
  sum_nodes(root);
  std::cout << root->value << '\n';
  return 0;
}
