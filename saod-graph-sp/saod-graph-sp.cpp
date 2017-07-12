#include <memory>
#include <iostream>
#include <list>   
#include <algorithm>
#include <map>
#include <iterator> 

using namespace std;

class Graph {

public:

	class Node {
	public:
		int index; // ����� �������
		double weight; // ��� �������
		Node(int idx, double weight = 0) : index(idx), weight(weight) { }; // �����������, �� ��������� ��� ������� = 0
		bool operator < (const Node & second) { return index < second.index; }; // �������� < ��������� ������ �� ������
		bool operator == (const Node & second) { return index == second.index; }; // �������� == ��������� ������ �� ������
		using Ptr = shared_ptr<Node>;
	};

	class Verge {
	public:
		Node::Ptr A; // ��������� �� ������ �������
		Node::Ptr B; // ��������� �� ������ �������
		double verge_weight; // ��� �����
		Verge(Node::Ptr A, Node::Ptr B, double weight) : A(A), B(B), verge_weight(weight) { }; // �����������
//verge(int A, int B, double weight) : node_A(make_shared<node>(new node(A))), node_B(make_shared<node>(new node(B))), verge_weight(weight) { }; // �����������
		bool operator < (const Verge & second) {
			return (A->index < second.A->index)
				|| (A->index == second.A->index
					&& B->index < second.B->index);
		};
		bool operator == (const Verge & second) {
			return A->index == second.A->index
				&& B->index == second.B->index;
		};
		using Ptr = shared_ptr<Verge>;
	};

	class Nodes {
	private:
		list<Node> _nodes;
	public:
		~Nodes() { _nodes.clear(); };
		bool Add(int idx);
		Node::Ptr Get(int idx);
		bool Del(int idx);
		void Print();
	};

	class Verges {
	private:
		list<Verge> _verges;
	public:
		~Verges() { _verges.clear(); };
		bool Add(int A_idx, int B_idx);
		Verge::Ptr Get(int A_idx, int B_idx);
		bool Del(int A_idx, int B_idx);
		void Print();
	};

	Graph() {};
	Graph(Graph const &G) = delete; // ������� ����������� �����������
	Graph(Graph &&G) = delete; // ������� ����������� �����������
	void *operator new(std::size_t) = delete; // ������� ��������� new
	void *operator new[](std::size_t) = delete; //

};



int main()
{

	Graph::Node::Ptr ps;




    //return 0;
}

