#pragma once

#include <limits>
#include <memory>
#include <iostream>
//#include <list>   
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
		int index; // Номер вершины
		double weight; // Вес вершины
		int level; // Уровень вершины (для алгоритма Демукрона)
		Node(int idx, double weight = 0) : index(idx), weight(weight), level(-1) { }; // Конструктор, по умолчанию вес вершины = 0, уровень = -1
		bool operator < (const Node & second) { return index < second.index; }; // Оператор < сравнения вершин по номеру
		bool operator == (const Node & second) { return index == second.index; }; // Оператор == сравнения вершин по номеру
		using Ptr = shared_ptr<Node>;
		struct Comparator {
			bool operator() (const Ptr &lhs, const Ptr &rhs) const {
				return lhs->index < rhs->index;
			}
		};
	};

	class Verge {
	public:
		Node::Ptr A; // Указатель на первую вершину
		Node::Ptr B; // Указатель на вторую вершину
		double weight; // Вес ребра
		Verge(Node::Ptr A, Node::Ptr B, double weight = 0) : A(A), B(B), weight(weight) { }; // Конструктор
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
		//list<Node::Ptr> _nodes;
		set<Node::Ptr, Node::Comparator> _nodes;
	public:
		Nodes(Graph *owner) : owner(owner) {};
		~Nodes() { _nodes.clear(); };
		//list<Node::Ptr> *List() { return &_nodes; };
		set<Node::Ptr, Node::Comparator> *Set() { return &_nodes; };
		Node::Ptr Get(int idx);
		//Node::Ptr operator[](int idx);
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

public:
	
	Graph() {};
	Graph(Graph const &G) = delete; // Удаляем конструктор копирования
	Graph(Graph &&G) = delete; // Удаляем конструктор перемещения
	void *operator new(std::size_t) = delete; // Удаляем операторы new
	void *operator new[](std::size_t) = delete; //

	Nodes nodes { this };
	Verges verges { this };

private:

	using ConnMap = std::unordered_map<int, std::unordered_map<int, double>>;
	ConnMap _conn_map; // Матрица смежности. _conn_map[i] - map<i, weight>
	void _re_map();

public:
	
	void Print_Connectivity_Matrix(); // Выводим граф в виде матрицы смежности	

};