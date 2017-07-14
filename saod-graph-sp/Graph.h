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
		int index; // Номер вершины
		double weight; // Вес вершины
		int level; // Уровень вершины (для алгоритма Демукрона)
		int indeg; // Полустепень захода
		int outdeg; // Полустепень исхода
		Node(int idx, double weight = 0) : index(idx), weight(weight), level(-1), indeg(0), outdeg(0) { }; // Конструктор, по умолчанию вес вершины = 0, уровень = -1
		bool operator < (const Node & second) { return index < second.index; }; // Оператор < сравнения вершин по номеру
		bool operator == (const Node & second) { return index == second.index; }; // Оператор == сравнения вершин по номеру
		using Ptr = shared_ptr<Node>;
		struct Comparator_ByIndex { // Функтор для сравнения узлов по их номеру
			bool operator() (const Ptr &lhs, const Ptr &rhs) const {
				return lhs->index < rhs->index;
			}
		};
		struct Comparator_ByLevel { // Функтор для сравнения узлов по их уровню
			bool operator() (const Ptr &lhs, const Ptr &rhs) const {
				return lhs->level < rhs->level;
			}
		};
	};

	class Verge {
	public:
		Node::Ptr A; // Указатель на первую вершину
		Node::Ptr B; // Указатель на вторую вершину
		double weight; // Вес ребра
		Verge(Node::Ptr A, Node::Ptr B, double weight = 0) : A(A), B(B), weight(weight) { }; // Конструктор
		bool operator < (const Verge & second) { // Оператор сравнения ребер по номерам вершин
			return (A->index < second.A->index) || (A->index == second.A->index && B->index < second.B->index);
		}
		bool operator == (const Verge & second) {
			return A->index == second.A->index && B->index == second.B->index;
		}
		using Ptr = shared_ptr<Verge>;
		struct Comparator { // Функтор для сравнения ребер по номерам вершин и весу ребер
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
		list<Node::Ptr> _nodes; // Лучше список - можно явно сортировать по номерам, уровням и пр.
					//set<Node::Ptr, Node::Comparator_ByIndex> _nodes;
	public:
		Nodes(Graph *owner) : owner(owner) {};
		~Nodes() { _nodes.clear(); };
		list<Node::Ptr> *List() { return &_nodes; };
					//set<Node::Ptr, Node::Comparator_ByIndex> *Set() { return &_nodes; };
		Node::Ptr Get(int idx); // Возращает SP ссылку на узел с номером idx
		//Node::Ptr operator[](int idx);
		Node::Ptr operator()(int idx); // Оператор (idx) возвращает SP ссылку на узел с номером idx
		bool Add(int idx, double); // Добавляет в список узел с номером idx и весом (по умолчанию 0)
		bool Del(int idx); // Удаляет из списка узел с номером idx
		void ReCalcDegrees(); // Пересчитывает полустепени захода и выхода узлов
		static void ReCalcDegrees(list<Node::Ptr> nodes, list<Verge::Ptr> verges);
		void Print(); // Выводит список узлов в cout
		static void Print(list<Node::Ptr> nodes);
		template<class _Comparator>
		void Sort(_Comparator Comparator) { // Сортирует список при помощи заданного функтора-компаратора
			_nodes.sort(Comparator);
		}
		list<Node::Ptr> Level(int level); // Возвращает новый список вершин  уровня level
		list<Node::Ptr> Nearby(int idx); // Возвращает новый список вершин, смежных к вершине idx
	};

	class Verges {
	private:
		list<Verge::Ptr> _verges; // Список лучше?
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
		void Sort(_Comparator Comparator) { // Сортирует список при помощи заданного функтора-компаратора
			_verges.sort(Comparator);
		}
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
	
	void PrintConnectivityMatrix(); // Выводим граф в виде матрицы смежности	
	void ReCalcNodesLevels(); // Пересчет уровней узлов
	
	struct dfs {
		Graph *owner;
		dfs(Graph *g): owner(g) {};
		bool operator () (int idx);
	};

};