/*
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
		list<Node::Ptr> _nodes;
	public:
		Nodes(Graph *owner): owner(owner) {};
		~Nodes() { _nodes.clear(); };
		list<Node::Ptr> *List() { return &_nodes; };

		Node::Ptr Get(int idx) { 
			Node::Ptr p;
			for (auto &it : _nodes)
				if ((*it).index == idx) {
					p = it;
					return p;
				};
			return p;
		}

		//Node::Ptr operator[](int idx) {
		Node::Ptr operator()(int idx) { 
			Node::Ptr sp = Get(idx);
			if (!sp) {
				sp.reset(new Node {idx});
				_nodes.push_back(sp);
				_nodes.sort(Node::Comparator());
			}
			return sp; 
		}


		bool Add(int idx, double weight = 0) {
			if (Get(idx))
				return false;
			_nodes.push_back(make_shared<Node>(idx, weight));
			_nodes.sort(Node::Comparator());
			return true;
		}
		
		bool Del(int idx) {
			if (owner->verges.IsNodeIncluded(idx))
				return false;
			for (auto &it : _nodes) 
				if (it->index == idx) {
					_nodes.remove(it);
					return true;
				}
			return false;
		}

		void Print() {
			for (const auto &it : _nodes)
				cout << it->index << "(" << it->weight << ")\t";
			cout << endl;
		}
	};

	class Verges {
	private:
		//list<Verge::Ptr> _verges;
		set<Verge::Ptr, Verge::Comparator> _verges;
		Graph *owner;
	public:
		Verges(Graph *owner) : owner(owner) {};
		~Verges() { _verges.clear(); };
		
		bool Add(int A_idx, int B_idx, double weight = 0) {
			Node::Ptr sp_A = owner->nodes(A_idx);
			Node::Ptr sp_B = owner->nodes(B_idx);
			//_verges.push_back(make_shared<Verge>(sp_A, sp_B, weight));
			//_verges.sort(Verge::Comparator());
			_verges.insert(make_shared<Verge>(sp_A, sp_B, weight));
			return true; //????????????????????????????????????????
		}

		Verge::Ptr Get(int A_idx, int B_idx) {
			Verge::Ptr sp;
			for (const auto &it : _verges) {
				if (it->A->index == A_idx && it->B->index == B_idx)
					sp = it;
			}
			return sp;
		}

		Verge::Ptr operator()(int A_idx, int B_idx) {
			Verge::Ptr sp = Get(A_idx, B_idx);
			if (!sp) {
				sp.reset(new Verge { owner->nodes(A_idx), owner->nodes(B_idx) });
				//_verges.push_back(sp);
				//_verges.sort(Verge::Comparator());
				_verges.insert(sp);
			}
			return sp;
		}


		bool Del(int A_idx, int B_idx) {
			for (auto &it : _verges) {
				if (it->A->index == A_idx && it->B->index == B_idx) {
					//_verges.remove(it);
					_verges.erase(it);
					return true;
				}
			}
			return false;
		}

		bool IsNodeIncluded(int idx) {
			for (auto &it : _verges)
				if (it->A->index == idx || it->B->index == idx)
					return true;
			return false;
		}

		void Print() {
			for (const auto &it : _verges)
				cout << "(" << it->A->index << ")-(" << it->B->index << ")[" << it->weight << "]\t";
			cout << endl;
		}
	};

	Graph() {};
	Graph(Graph const &G) = delete; // Удаляем конструктор копирования
	Graph(Graph &&G) = delete; // Удаляем конструктор перемещения
	void *operator new(std::size_t) = delete; // Удаляем операторы new
	void *operator new[](std::size_t) = delete; //

	Nodes nodes {this};
	Verges verges {this};



	using ConnMap = std::map<int, std::map<int, double>>;
	ConnMap _conn_map; // Матрица смежности. _conn_map[i] - map<i, weight>

	void _re_map() {
		_conn_map.clear();
		for (const auto &n_col : *nodes.List())
			for (const auto &n_row : *nodes.List()) {
				auto v = verges.Get(n_col->index, n_row->index);
				if (v)
					_conn_map[n_col->index][n_row->index] = v->weight;
				else
					_conn_map[n_col->index][n_row->index] = Inf;  // Infinity
			}
	};

	void Print_Connectivity_Matrix() { // Выводим граф в виде матрицы смежности
		_re_map();
		// Header:
		cout << "[ ]\t";
		for (auto iter : _conn_map)
			cout << "[" << iter.first << "]\t";
		cout << endl;
		// Rows:
		for (auto iter : _conn_map) {
			cout << "[" << iter.first << "]\t";
			for (auto iter2 : iter.second) {
				(iter2.second == Inf) ? cout << "-\t" : cout << iter2.second << "\t";
			}
			cout << endl;
		}
	}

};


*/
#include "Graph.h"

int main()
{
	Graph G;

	G.nodes.Add(3, 300);
	G.nodes.Add(1, 100);
	G.nodes.Add(1, 1000);
	G.nodes.Add(5, 500);
	G.nodes.Add(2, 200);
	G.nodes.Del(2);
	G.nodes.Print();

	G.nodes(1)->weight = 1100;
	G.nodes(11)->weight = 1111;
	G.nodes.Sort(Graph::Node::Comparator_ByIndex());
	G.nodes.Print();
	cout << endl;

	G.verges.Add(1, 3, 103);
	G.verges.Add(1, 5, 105);
	G.verges(22, 33)->weight = 333;
	cout << endl;
	
	G.verges.Print();
	G.nodes.Print();
	cout << endl;

	G.nodes.ReCalcDegrees();
	G.nodes.Print();
	cout << endl;

	G.PrintConnectivityMatrix();
	cout << endl;
	cout << endl;
	cout << "-------------------------------------------" << endl;

	Graph G2;
	G2.verges(1, 4)->weight = 1.0;
	G2.verges(1, 5)->weight = 2.0;
	G2.verges(3, 6)->weight = 3.0;
	G2.verges(4, 7)->weight = 4.0;
	G2.verges(5, 7)->weight = 5.0;
	G2.verges(6, 7)->weight = 6.0;
	G2.verges(6, 9)->weight = 7.0;
	G2.verges(11, 5)->weight = 8.0;
	G2.verges(11, 12)->weight = 9.0;
	G2.verges(9, 13)->weight = 10.0;
	G2.verges(12, 13)->weight = 11.0;
	

	G2.ReCalcNodesLevels();
	cout << endl;
	cout << "-------------------------------------------" << endl;
	G2.nodes.Print();
	cout << "===========================================" << endl;
	Graph::dfs dfs2 { &G2 };
	//cout << dfs2(1) << endl;
	cout << "DFS demo:" << endl;
	cout << dfs2.topological_sort() << endl;
	dfs2._print_stack();

	Graph::bfs bfs1 { &G2 };
	bfs1(11);
	cout << "BFS demo:" << endl;
	bfs1._print_Distance();

	Graph::dijkstra ds { &G2 };
	ds(11);
	cout << "Dijkstra demo:" << endl;
	ds._print_Distance();

	// The end
	cout << endl << "\n\nEnter to exit...";
	cin.get();
}

