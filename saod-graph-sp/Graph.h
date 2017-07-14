#pragma once

#include <limits>
#include <memory>
#include <iostream>
#include <list>   
#include <unordered_map>
#include <set>
//#include <unordered_set>

//#include <iterator> 
//#include <algorithm>

using namespace std;

class Graph {

public:

	static const auto Inf = INT_MAX;

	class Node {
	public:
		int index; // ����� �������
		double weight; // ��� �������
		int level; // ������� ������� (��� ��������� ���������)
		int indeg; // ����������� ������
		int outdeg; // ����������� ������
		Node(int idx, double weight = 0) : index(idx), weight(weight), level(-1), indeg(0), outdeg(0) { }; // �����������, �� ��������� ��� ������� = 0, ������� = -1
		bool operator < (const Node & second) { return index < second.index; }; // �������� < ��������� ������ �� ������
		bool operator == (const Node & second) { return index == second.index; }; // �������� == ��������� ������ �� ������
		using Ptr = shared_ptr<Node>;
		struct Comparator_ByIndex { // ������� ��� ��������� ����� �� �� ������
			bool operator() (const Ptr &lhs, const Ptr &rhs) const {
				return lhs->index < rhs->index;
			}
		};
		struct Comparator_ByLevel { // ������� ��� ��������� ����� �� �� ������
			bool operator() (const Ptr &lhs, const Ptr &rhs) const {
				return lhs->level < rhs->level;
			}
		};
	};

	class Verge {
	public:
		Node::Ptr A; // ��������� �� ������ �������
		Node::Ptr B; // ��������� �� ������ �������
		double weight; // ��� �����
		Verge(Node::Ptr A, Node::Ptr B, double weight = 0) : A(A), B(B), weight(weight) { }; // �����������
		bool operator < (const Verge & second) { // �������� ��������� ����� �� ������� ������
			return (A->index < second.A->index) || (A->index == second.A->index && B->index < second.B->index);
		}
		bool operator == (const Verge & second) {
			return A->index == second.A->index && B->index == second.B->index;
		}
		using Ptr = shared_ptr<Verge>;
		struct Comparator { // ������� ��� ��������� ����� �� ������� ������ � ���� �����
			bool operator() (const Ptr &lhs, const Ptr &rhs) const {
				return (lhs->A->index < rhs->A->index) 
					|| (lhs->A->index == rhs->A->index && lhs->B->index < rhs->B->index)
					|| (lhs->A->index == rhs->A->index && lhs->B->index == rhs->B->index) && lhs->B->weight == rhs->B->weight;
			}
		};
	};

	class Nodes {
	private:
		Graph *owner;
		list<Node::Ptr> _nodes; // ����� ������ - ����� ���� ����������� �� �������, ������� � ��.
					//set<Node::Ptr, Node::Comparator_ByIndex> _nodes;
	public:
		Nodes(Graph *owner) : owner(owner) {};
		~Nodes() { _nodes.clear(); };
		list<Node::Ptr> *List() { return &_nodes; };
					//set<Node::Ptr, Node::Comparator_ByIndex> *Set() { return &_nodes; };
		Node::Ptr Get(int idx); // ��������� SP ������ �� ���� � ������� idx
		//Node::Ptr operator[](int idx);
		Node::Ptr operator()(int idx); // �������� (idx) ���������� SP ������ �� ���� � ������� idx
		bool Add(int idx, double); // ��������� � ������ ���� � ������� idx � ����� (�� ��������� 0)
		bool Del(int idx); // ������� �� ������ ���� � ������� idx
		void ReCalcDegrees(); // ������������� ����������� ������ � ������ �����
		static void ReCalcDegrees(list<Node::Ptr> nodes, list<Verge::Ptr> verges);
		void Print(); // ������� ������ ����� � cout
		static void Print(list<Node::Ptr> nodes);
		template<class _Comparator>
		void Sort(_Comparator Comparator) { // ��������� ������ ��� ������ ��������� ��������-�����������
			_nodes.sort(Comparator);
		}
		list<Node::Ptr> Level(int level); // ���������� ����� ������ ������  ������ level
		list<Node::Ptr> Nearby(int idx); // ���������� ����� ������ ������, ������� � ������� idx
	};

	class Verges {
	private:
		list<Verge::Ptr> _verges; // ������ �����?
		//set<Verge::Ptr, Verge::Comparator> _verges;
		Graph *owner;
	public:
		Verges(Graph *owner) : owner(owner) {};
		~Verges() { _verges.clear(); };
		//set<Verge::Ptr, Verge::Comparator> *Set() { return &_verges; };
		list<Verge::Ptr> *List() { return &_verges; };
		bool Add(int A_idx, int B_idx, double weight);
		Verge::Ptr Get(int A_idx, int B_idx);
		Verge::Ptr operator()(int A_idx, int B_idx);
		bool Del(int A_idx, int B_idx);
		bool IsNodeLinked(int idx);
		void Print();
		template<class _Comparator>
		void Sort(_Comparator Comparator) { // ��������� ������ ��� ������ ��������� ��������-�����������
			_verges.sort(Comparator);
		}
	};

public:
	
	Graph() {};
	Graph(Graph const &G) = delete; // ������� ����������� �����������
	Graph(Graph &&G) = delete; // ������� ����������� �����������
	void *operator new(std::size_t) = delete; // ������� ��������� new
	void *operator new[](std::size_t) = delete; //

	Nodes nodes { this };
	Verges verges { this };

private:

	using ConnMap = std::unordered_map<int, std::unordered_map<int, double>>;
	ConnMap _conn_map; // ������� ���������. _conn_map[i] - map<i, weight>
	void _re_map();

public:
	
	void PrintConnectivityMatrix(); // ������� ���� � ���� ������� ���������	
	void ReCalcNodesLevels(); // �������� ������� �����
	
	struct dfs {
		Graph *owner;
		dfs(Graph *g): owner(g) {};
		bool operator () (int idx);
	};

};