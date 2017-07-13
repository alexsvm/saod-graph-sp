#pragma once

#include <limits>
#include <memory>
#include <iostream>
#include <list>   
#include <map>
#include <set>
#include <unordered_set>
#include <iterator> 

#include <algorithm>

using namespace std;

class Graph {

public:

	static const auto Inf = INT_MAX;

	class Node {
	public:
		int index; // ����� �������
		double weight; // ��� �������
		int level; // ������� ������� (��� ��������� ���������)
		Node(int idx, double weight = 0) : index(idx), weight(weight), level(-1) { }; // �����������, �� ��������� ��� ������� = 0, ������� = -1
		bool operator < (const Node & second) { return index < second.index; }; // �������� < ��������� ������ �� ������
		bool operator == (const Node & second) { return index == second.index; }; // �������� == ��������� ������ �� ������
		using Ptr = shared_ptr<Node>;
		struct Comparator {
			bool operator() (const Ptr &lhs, const Ptr &rhs) const {
				return lhs->index < rhs->index;
			}
		};
	};

	class Verge {
	public:
		Node::Ptr A; // ��������� �� ������ �������
		Node::Ptr B; // ��������� �� ������ �������
		double weight; // ��� �����
		Verge(Node::Ptr A, Node::Ptr B, double weight = 0) : A(A), B(B), weight(weight) { }; // �����������
		bool operator < (const Verge & second) {
			return (A->index < second.A->index) || (A->index == second.A->index && B->index < second.B->index);
		};
		bool operator == (const Verge & second) {
			return A->index == second.A->index && B->index == second.B->index;
		};
		using Ptr = shared_ptr<Verge>;
		struct Comparator {
			bool operator() (const Ptr &lhs, const Ptr &rhs) const {
				return (lhs->A->index < rhs->A->index) || (lhs->A->index == rhs->A->index && lhs->B->index < rhs->B->index);
			};
		};
	};

	class Nodes {
	private:
		Graph *owner;
		list<Node::Ptr> _nodes;
	public:
		Nodes(Graph *owner) : owner(owner) {};
		~Nodes() { _nodes.clear(); };
		list<Node::Ptr> *List() { return &_nodes; };
		Node::Ptr Get(int idx);
		//Node::Ptr operator[](int idx) {
		Node::Ptr operator()(int idx);
		bool Add(int idx, double);
		bool Del(int idx);
		void Print();
	};

	class Verges {
	private:
		//list<Verge::Ptr> _verges;
		set<Verge::Ptr, Verge::Comparator> _verges;
		Graph *owner;
	public:
		Verges(Graph *owner) : owner(owner) {};
		~Verges() { _verges.clear(); };
		bool Add(int A_idx, int B_idx, double weight);
		Verge::Ptr Get(int A_idx, int B_idx);
		Verge::Ptr operator()(int A_idx, int B_idx);
		bool Del(int A_idx, int B_idx);
		bool IsNodeIncluded(int idx);
		void Print();
	};

	Graph() {};
	Graph(Graph const &G) = delete; // ������� ����������� �����������
	Graph(Graph &&G) = delete; // ������� ����������� �����������
	void *operator new(std::size_t) = delete; // ������� ��������� new
	void *operator new[](std::size_t) = delete; //

	Nodes nodes{ this };
	Verges verges{ this };

	using ConnMap = std::map<int, std::map<int, double>>;
	ConnMap _conn_map; // ������� ���������. _conn_map[i] - map<i, weight>

	void _re_map();

	void Print_Connectivity_Matrix(); // ������� ���� � ���� ������� ���������
		

};