#include <iostream>
#include <vector>

using namespace std;

// 조형물 정보와 산책로 디자인 비용이 들어가는 노드(연결되어있는 정점과 간선이 들어 있는 노드)
class Node{
public:
    // 조형물 정보
    int adjvertex;
    // 산책로 디자인 비용
    int price;
    // 노드 생성자
    Node(int adj, int pri) {
        adjvertex = adj;
        price = pri;
    }
};

// Prim 알고리즘에서 Fringe 사용을 위해 Heap구조의 Priority Queue생성
class Heap{
public:
    // Heap 관리를 위한 vector
    vector<Node*> h;
    // Heap size
    int size;
    // Heap의 root 인덱스 1 (0은 비워두는 자리)
    int root;

    // Heap 생성자
    Heap() {
        // h[0]에 의미없는 것을 채우기 위함
        Node* null_ = new Node(-1,-1);
        h.push_back(null_);
        size = 0;
        root = 1;
    }
    // upHeap 혹은 downHeap에서 서로 바꿀때 사용
    void Swap(int id1, int id2) {
        Node* temp = h[id1];
        h[id1] = h[id2];
        h[id2] = temp;
    }

    // Heap의 삽입을 위한 upheap수행
    void upHeap(int id) {
        if (id == root) {
            return;
        }
        else {
            int par = id/2;
            // Heap 구조를 유지하기 위해 swap해준다
            if(h[par]->price > h[id]->price){
                Swap(par, id);
                upHeap(par);
            }
            // 산책로 디자인 비용이 같으면 조형물 번호가 작은 순으로 바꿔준다
            else if(h[par]->price == h[id]->price){
                if(h[par]->adjvertex > h[id]->adjvertex){
                    Swap(par, id);
                    upHeap(par);
                }
            }
        }
    }

    // Heap의 삭제를 위한 downHeap
    void downHeap(int id) {
        int left = id * 2;
        int right = (id * 2) + 1;

        // 자식이 둘 다 있을 떄
        if(right <= size) {
            // 왼쪽 자식이 오른쪽 자식보다 산책로 비용이 적을 때
            if(h[left]->price < h[right]->price) {
                if(h[left]->price < h[id]->price) {
                    Swap(left, id);
                    downHeap(left);
                }
            }
            // 오른쪽 자식이 왼쪽 자식보다 산책로 비용이 적을 때
            else if(h[left]->price > h[right]->price) {
                if(h[right]->price < h[id]->price){
                    Swap(right, id);
                    downHeap(right);
                }
            }
            // 왼쪽 자식과 오른쪽 자식의 산책로 비용이 같을 때 : 조형물 번호 순으로 바꿔준다
            else {
                // 오른쪽 자식의 조형물 번호가 왼쪽 자식의 조형물 번호보다 작을 때
                if(h[right]->adjvertex < h[left]->adjvertex) {
                    // 오른쪽 자식이 자신의 산책로 비용보다 작을 떄
                    if(h[right]->price < h[id]->price){
                        Swap(right, id);
                        downHeap(right);
                    }
                    // 오른쪽 자식이 자신의 산책로 비용과 같을 때 : 조형물 번호 순으로 바꿔준다
                    else if(h[right]->price == h[id]->price) {
                        if(h[right]->adjvertex < h[id]->adjvertex) {
                            Swap(right, id);
                            downHeap(right);
                        }
                    }
                }
                // 왼쪽 자식의 조형물 번호가 왼쪽 자식의 조형물 번호보다 작을 때
                else{
                    // 왼쪽 자식이 자신의 산책로 비용보다 작을 때
                    if(h[left]->price < h[id]->price) {
                        Swap(left, id);
                        downHeap(left);
                    }
                    // 왼쪽 자식이 자신의 산책로 비용과 같을 때 : 조형물 번호 순으로 바꿔준다
                    else if(h[left]->price == h[id]->price) {
                        if(h[left]->adjvertex < h[id]->adjvertex) {
                            Swap(left, id);
                            downHeap(left);
                        }
                    }
                }
            }
        }
        // 자식이 하나만 있을 때
        else if(left <= size) {
            // 왼쪽 자식이 자신의 산책로 비용보다 작을 때
            if(h[left]->price < h[id]->price) {
                Swap(left, id);
                downHeap(left);
            }
            // 왼쪽 자식이 자신의 산책로 비용과 같을 때 : 조형물 번호 순으로 바꿔준다
            else if(h[left]->price == h[id]->price) {
                if(h[left]->adjvertex < h[id]->adjvertex) {
                    Swap(left, id);
                    downHeap(left);
                }
            }
        }
        // 자식이 없을 때
        else {
            return;
        }
    }

    // Heap 삽입연산
    void insert(Node* node) {
        // Heap에(Fringe)에 같은 정점이 있는 지 확인
        int id_ = search(node);
        // 같은 정점이 있을 때 : 새로 들어오려하는 정점의 비용이 기존 비용보다 작다면 바꿔준다
        if(id_ != 0) {
            if(h[id_]->price > node->price) {
                h[id_]->price = node->price;
                // 바꿔주면 부모보다 비용이 주는 경우가 있을 수 있으므로 upheap 수행
                upHeap(id_);
            }
        }
        else {
            h.push_back(node);
            size++;
            // upheap 수행
            upHeap(size);
        }

    }

    // Heap의 삭제 연산
    Node* pop() {
        Node* top = h[root];
        h[root] = h[size];
        size--;
        h.pop_back();
        downHeap(root);
        return top;
    }

    // 같은 정점(조형물)이 있는지 확인 하는 함수
    int search(Node* node) {
        for(int i = 1; i < size+1; i++) {
            if (node->adjvertex == h[i]->adjvertex) {
                return i;
            }
        }
        return 0;
    }
};

// 그래프 (Prim 알고리즘 포함)
class graph{
public:
    // 그래프를 관리할 2차원 벡터
    vector<Node*>* g;
    // 조형물 추가
    explicit graph(int n){
        // 2차원 벡터의 행은 조형물(정점)
        // 2차원 벡터의 열은 연결되어 있는 조형물과 산책로 비용
        // 따라서 열이 간선 리스트이다.
        g = new vector<Node*>[n];

    }

    // 산책로 추가(간선추가)
    void insert_Edge(int n1, int n2, int price){
        Node* n1_ = new Node(n2, price);
        Node* n2_ = new Node(n1, price);

        g[n1].push_back(n1_);
        g[n2].push_back(n2_);
    }

    // Prim 알고리즘에서 Tree에 있는 정점의 인접 정점과 비용을 반환 함수
    vector<Node*> adj(int id) {
        // 인접한 정점+비용을 vector로 관리
        vector<Node*> v;
        for(unsigned int i = 0; i < g[id].size(); i++) {
            Node* n1_ = new Node(g[id][i]->adjvertex, g[id][i]->price);
            v.push_back(n1_);
        }
        return v;
    }

    // 모든 조형물을 최소 비용으로 연결하기 위해 Prim 알고리즘 사용
    // 사용자가 입력한 조형물 번호로 시작
    // 다음 조형물을 연결하기 위한 가장 작은 비용이 드는 조형물을 선택 알고리즘
    // Tree 와 Fringe로 관리
    // 인접 리스트 방식 그래프 구현, Heap 으로 Priority Queue구현
    // step : n개
    // 인접한 edge를 처리하는 cost = O(deg(v)) : tree, fringe, unseen 에서 O(1)이므로 총 O(deg(v))
    // Heap에서 pop연산 1번 cose = O(log(n))
    // 따라서 이 Prim 알고리즘의 총비용은 n*(O(deg(v) + O(log(n)))
    // n*(O(m) + O(log(n)) = O(n log(n))이다

    void prim(int id) {
        int W = 0;
        // tree set 관리를 위해 vector 선언 조형물 번호만 관리
        // 추후에 트리 set에 추가되는 순서로 출력
        vector<int> tree;

        // Prim 알고리즘의 핵심
        // 우선순위큐(heap)으로 관리해서 비용이 최소인 간선을 구함
        Heap fringe;

        // Tree에 있는 정점인지 확인하기 위해 bool형 배열 선언
        // 1 : Tree o 0 : Tree x
        bool visited[10001] = {0,};

        // 시작 정점 tree에 추가
        tree.push_back(id);
        visited[id] = true;

        // 시작 정점의 인접 정점들
        vector<Node*> v;
        v = adj(id);

        // 인점 정점들을 Fringe로 이동
        for(unsigned int i = 0; i < v.size(); i++) {
            fringe.insert(v[i]);
        }

        // Heap의 pop을 이용해서 prim 알고리즘 구현
        while(fringe.size != 0) {
            // 가장 비용이 적게 드는 경우 선택
            Node* curNode = fringe.pop();

            // 트리에 삽입
            tree.push_back(curNode->adjvertex);
            visited[curNode->adjvertex] = true;

            // 총비용 계산
            W = W + curNode->price;

            // 트리에 들어간 정점의 인점 정점들
            vector<Node*> v_;
            v_ = adj(curNode->adjvertex);

            for(unsigned int i = 0; i < v_.size(); i++) {
                // tree에 없으면 fringe set에 삽입
                if(!visited[v_[i]->adjvertex]){
                    fringe.insert(v_[i]);
                }
            }
        }
        // 총비용 출력
        cout << W << " ";
        // 트리에 들어간 순서대로 출력
        for(unsigned int i = 0; i < tree.size(); i++) {
            if(i == tree.size()-1) {
                cout << tree[i] << endl;
            }
            else {
                cout << tree[i] << " ";
            }
        }
    }
};

// 메인 함수
int main() {
    // 조형물 정보의 수, 산책로 정보의 수, 질의의 수 입력
    int n, m, q;
    cin >> n >> m >> q;

    // 그래프 생성 및 조형물 추가
    graph a(n+1);

    // 조형물 사이의 산책로 정보 입력
    while(m--) {
        int n1, n2, price;
        cin >> n1 >> n2 >> price;
        // 산책로 추가
        a.insert_Edge(n1, n2, price);
    }
    // 질의 입력
    while(q--) {
        char w;
        cin >> w;
        // 산책로 디자인 총비용 및 Prim 알고리즘 진행과정 출력
        if (w == 'P') {
            int S;
            cin >> S;
            // Prim 알고리즘 호출
            a.prim(S);
        }
    }
    return 0;
}

