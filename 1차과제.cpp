#include <iostream>
using namespace std;

class Node{
private:
    // 애플리케이션 정보들
    int id;
    string name;
    int capacity;
    int price;

    // 레드 블랙 트리를 구성하기 위한 포인터들
    Node* par;
    Node* left;
    Node* right;

    // 레드 블랙 트리 노드의 색 : Black-0, Red-1
    int color;

public:
    // 노드 생성자 (하나의 어플리케이션)
    Node(int id_, string name_, int capacity_, int price_) {
        this->id = id_;
        this->name = name_;
        this->capacity = capacity_;
        this->price = price_;
        this->color = 1;

        this->par = nullptr;
        this->left = nullptr;
        this->right = nullptr;

    }
    // 노드의 color를 Black으로 바꿔주는 함수
    void color_RB_Chagne() {
        this->color = 0;
    }
    // 노드의 color를 Red로 바꿔주는 함수
    void color_BR_Change() {
        this->color = 1;
    }

    friend class RB_Tree;
};

class RB_Tree{
private:
    Node* root;

public:
    // Red-Black 트리 생성자
    RB_Tree() {
        this->root = nullptr;
    }

    // 로트 노드 반환 함수 (discount 함수를 재귀적으로 만들어서 main함수에서 필요)
    Node* get_Root(){
        return root;
    }

    // sibling node의 색을 반환하는 함수
    int getSiblingcolor(Node* node) {
        if((node->par) == (node->par->par->left)) {
            if(node->par->par->right == nullptr) {
                return 0;
            }
            else {
                return node->par->par->right->color;
            }
        }
        else{
            if(node->par->par->left == nullptr) {
                return 0;
            }
            else {
                return node->par->par->left->color;
            }
        }
    }

    // doubleRed 함수
    void doubleRed(Node* node) {

        // doubleRed 일 때
        if(node->par->color == 1) {
            // Recoloring
            if(getSiblingcolor(node) == 1) {
                if(node->par->par != root) {
                    node->par->par->color_BR_Change();

                    node->par->par->left->color_RB_Chagne();
                    node->par->par->right->color_RB_Chagne();
                    doubleRed(node->par->par);
                }
                else {
                    node->par->par->left->color_RB_Chagne();
                    node->par->par->right->color_RB_Chagne();
                }

            }
            // Restructuring
            else{
                Node* gparNode = node->par->par;
                Node* parNode = node->par;

                // 경우 1 : 3 - 2 - 1
                if((gparNode->left == parNode) && (parNode->left == node)) {
                    gparNode->left = parNode->right;

                    if(parNode->right != nullptr) {
                        parNode->right->par = gparNode;
                    }
                    parNode->right = gparNode;
                    parNode->par = gparNode->par;

                    if(gparNode != root) {
                        parNode->par = gparNode->par;
                        if(gparNode->id < gparNode->par->id) {
                            gparNode->par->left = parNode;
                        }
                        else {
                            gparNode->par->right = parNode;
                        }
                    }
                    else {
                        parNode->par = nullptr;
                        root = parNode;
                    }


                    gparNode->par = parNode;
                    parNode->color_RB_Chagne();
                    gparNode->color_BR_Change();

                }
                // 경우 2 : 3 - 1 - 2
                else if((gparNode->left == parNode) && (parNode->right == node)) {
                    gparNode->left = node->right;
                    if(node->right != nullptr) {
                        node->right->par = gparNode;
                    }

                    node->right = gparNode;

                    if(gparNode != root) {
                        node->par = gparNode->par;
                        if(gparNode->id < gparNode->par->id) {
                            gparNode->par->left = node;
                        }
                        else {
                            gparNode->par->right = node;
                        }
                    }
                    else {
                        node->par = nullptr;
                        root = node;
                    }


                    gparNode->par = node;

                    parNode->right = node->left;
                    if(node->left != nullptr){
                        node->left->par = parNode;
                    }
                    node->left = parNode;
                    parNode->par = node;


                    node->color_RB_Chagne();
                    gparNode->color_BR_Change();

                }
                // 경우 3 : 1 - 2 - 3
                else if((gparNode->right == parNode) && (parNode->right == node)) {
                    gparNode->right = parNode->left;

                    if(parNode->left != nullptr) {
                        parNode->left->par = gparNode;
                    }
                    parNode->left = gparNode;


                    if(gparNode != root) {
                        parNode->par = gparNode->par;
                        if(gparNode->id < gparNode->par->id) {
                            gparNode->par->left = parNode;
                        }
                        else {
                            gparNode->par->right = parNode;
                        }
                    }
                    else {
                        parNode->par = nullptr;
                        root = parNode;
                    }

                    gparNode->par = parNode;

                    parNode->color_RB_Chagne();
                    gparNode->color_BR_Change();

                }
                // 경우 4 : 3 - 1 -2
                else {
                    gparNode->right = node->left;
                    if(node->left != nullptr) {
                        node->left->par = gparNode;
                    }
                    node->left = gparNode;

                    if(gparNode != root) {
                        node->par = gparNode->par;
                        if(gparNode->id < gparNode->par->id) {
                            gparNode->par->left = node;
                        }
                        else {
                            gparNode->par->right = node;
                        }
                    }
                    else {
                        node->par = nullptr;
                        root = node;
                    }


                    gparNode->par = node;

                    parNode->left = node->right;
                    if(node->right != nullptr){
                        node->right->par = parNode;
                    }

                    node->right = parNode;
                    parNode->par = node;


                    node->color_RB_Chagne();
                    gparNode->color_BR_Change();
                }
            }
        }
    }

    //애플리케이션 등록 함수
    void insert(int id_, string name_, int capacity_, int price_) {
        Node* newNode = new Node(id_, name_, capacity_, price_);
        int depth = 0;

        // 트리가 비워져 있으면 root로 지정
        if(root == nullptr) {
            newNode->color_RB_Chagne();
            root = newNode;
            cout << depth << endl;
            return;
        }

        else {
            Node* parNode;
            Node* curNode = root;

            // 들어갈 자리 찾기 : 이미 있는 애플리케이션이면 depth 출력
            while(curNode != nullptr) {
                if(id_ == curNode->id) {
                    cout << depth << endl;
                    return;
                }
                else if(id_ > curNode->id) {
                    parNode = curNode;
                    curNode = curNode->right;
                    depth++;
                }
                else {
                    parNode = curNode;
                    curNode = curNode->left;
                    depth++;
                }
            }

            // 노드 넣기
            if(id_ < parNode->id) {
                parNode->left = newNode;
                newNode->par = parNode;
                // doubleRed check
                doubleRed(newNode);
            }
            else {
                parNode->right = newNode;
                newNode->par = parNode;
                // doubleRed check
                doubleRed(newNode);
            }

        }


        // 등록 후 최종 depth 출력
        Node* curNode = root;
        depth = 0;
        while(curNode != nullptr) {
            if(id_ == curNode->id) {
                cout << depth << endl;

                return;
            }
            else if(id_ > curNode->id) {
                curNode = curNode->right;
                depth++;
            }
            else {
                curNode = curNode->left;
                depth++;
            }
        }

    }

    //애플리케이션 검색 함수
    void Find(int id_){
        Node* curNode = root;
        int depth = 0;


        while(curNode != nullptr) {
            if (id_ == curNode->id) {
                cout << depth << " " << curNode->name << " " << curNode->capacity << " " << curNode->price << endl;
                return;
            }
            if(id_ > curNode->id) {
                curNode = curNode->right;
                depth++;
            }
            else{
                curNode = curNode->left;
                depth++;
            }

        }

        cout << "NULL" << endl;
   }

    //애플리케이션 업데이트 함수
    void update(int id_, string name_, int capacity_, int price_){
        Node* curNode = root;
        int depth = 0;

        while(curNode != nullptr) {
            if (id_ == curNode->id) {
                curNode->name = name_;
                curNode->capacity = capacity_;
                curNode->price = price_;
                cout << depth << endl;
                return;
            }
            if(id_ > curNode->id) {
                curNode = curNode->right;
                depth++;
            }
            else{
                curNode = curNode->left;
                depth++;
            }

        }

        cout << "NULL" << endl;
  }

    //애플리케이션 할인 함수
    void discount(Node* node, int x, int y, int P) {

            // inorder 순회
            if(node->left != nullptr) {
                discount(node->left, x, y, P);
            }
            // 범위에 맞는 지 확인
            if((node->id >= x) && (node->id <= y)) {
                node->price = (node->price * (100-P))/100;
            }
            
            if(node->right != nullptr) {
                discount(node->right, x, y, P);
            }

    }
};

int main() {

    // 사용한 기능 수 입력
    int n;
    cin >> n;

    // 비워져있는 레드 블랙 트리 생성
    RB_Tree a;

    // 기능 수 만큼 수행
    while(n--) {
        // 기능 입력
        char function;
        cin >> function;

        // 애플리케이션 등록
        if(function == 'I') {
            int id;
            string name;
            int capacity;
            int price;

            cin >> id >> name >> capacity >> price;
           a.insert(id, name, capacity, price);

        }
        // 애플리케이션 검색
        else if(function == 'F') {
            int id;

            cin >> id;
            a.Find(id);
        }
        // 애플리케이션 업데이트
        else if(function == 'R') {
            int id;
            string name;
            int capa;
            int price;

            cin >> id >> name >> capa >> price;
            a.update(id, name, capa, price);
        }
        // 애플리케이션 할인
        else if(function == 'D') {
            int x;
            int y;
            int P;
            cin >> x >> y >> P;
            a.discount(a.get_Root(), x, y, P);
        }
    }
    return 0;
}
