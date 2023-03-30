#include <iostream>
#include <queue>

// 편의상 한 직원은 최대 두 명의 부하 직원을 거느릴 수 있다고 가정
// 이진 트리

struct node {
  std::string position; // 직책
  node* first;
  node* second;
};

// 각각의 노드는 다른 두 개의 노드에 대한 링크를 가짐
// 이를 통해 데이터의 계층 구조 나타냄

struct org_tree {
  node* root; // CEO
  
  static org_tree create_org_structure(const std::string& pos) {
    org_tree tree;
    tree.root = new node {pos, nullptr, nullptr};
    return tree;
  }
  
  // 검색 ( 전위 순회 )
  static node* find(node* root, const std::string& value) {
    if(!root) return nullptr;
    if(root->position == value) return root;

    node* firstFound = org_tree::find(root->first, value);
    if (firstFound) return firstFound;

    node* secondFound = org_tree::find(root->second, value);
    return secondFound;
  }

  // 전위 순회 (preOrder)
  // 자식, 왼쪽, 오른쪽
  static void preOrder(node* start) {
    if (!start) return;

    std::cout << start->position << ", ";
    preOrder(start->first);
    preOrder(start->second);
  }

  // 중위 순회 (inOrder)
  static void inOrder(node* start) {
    if (!start) return;

    inOrder(start->first);
    std::cout << start->position << ", ";
    inOrder(start->second);
  }

  // 후위 순회 (postOrder)
  static void postOrder(node* start) {
    if (!start) return;

    postOrder(start->first);
    postOrder(start->second);
    std::cout << start->position << ", ";
  }

  // 레벨 순서 순회 (levelOrder)
  static void levelOrder(node* start) {
    std::queue<node*> q;
    q.push(start);

    while(true) {
      if(q.empty()) break;

      int size = q.size();
      for(int i = 0; i < size; i++) {
        auto current = q.front();
        q.pop();

        std::cout << current->position << ", ";
        if(current->first) q.push(current->first);
        if(current->second) q.push(current->second);
      }
      std::cout << std::endl;
    }
  }

  // 아래 조직 추가
  bool addSubOrdinate(const std::string& manager, const std::string& subOrdinate) {
    auto managerNode = org_tree::find(root, manager);

    if (!managerNode) {
      std::cout << manager << " 을(를) 찾을 수 없습니다." << std::endl;
      return false;
    }
    if(managerNode->first && managerNode->second) {
      std::cout << manager << " 아래에 " << subOrdinate << "을(를) 추가할 수 없습니다." << std::endl;
      return false;
    }

    if (!managerNode->first)
      managerNode->first = new node{subOrdinate, nullptr, nullptr};
    else
      managerNode->second = new node{subOrdinate, nullptr, nullptr};

    std::cout << manager << " 아래에 " << subOrdinate << "을(를) 추가했습니다."
              << std::endl;

    return true;
  }
};

int main() {
  auto tree = org_tree::create_org_structure("CEO");

  tree.addSubOrdinate("CEO", "부사장");
  tree.addSubOrdinate("부사장", "IT부장");
  tree.addSubOrdinate("부사장", "마케팅부장");
  tree.addSubOrdinate("IT부장", "보안팀장");
  tree.addSubOrdinate("IT부장", "앱개발팀장");
  tree.addSubOrdinate("마케팅부장", "물류팀장");
  tree.addSubOrdinate("마케팅부장", "홍보팀장");
  tree.addSubOrdinate("부사장", "재무부장");

  std::cout << std::endl;

  org_tree::levelOrder(tree.root);

}
