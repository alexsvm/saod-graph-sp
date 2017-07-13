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
		int index; // Номер вершины
		double weight; // Вес вершины
		Node(int idx, double weight = 0) : index(idx), weight(weight) { }; // Конструктор, по умолчанию вес вершины = 0
		bool operator < (const Node & second) { return index < second.index; }; // Оператор < сравнения вершин по номеру
		bool operator == (const Node & second) { return index == second.index; }; // Оператор == сравнения вершин по номеру
		using Ptr = shared_ptr<Node>;
		struct Comparator
		{
			bool operator() (const Ptr &lhs, const Ptr &rhs) const
			{
				return lhs->index < rhs->index;
			}
		};
	};

	class Verge {
	public:
		Node::Ptr A; // Указатель на первую вершину
		Node::Ptr B; // Указатель на вторую вершину
		double verge_weight; // Вес ребра
		Verge(Node::Ptr A, Node::Ptr B, double weight = 0) : A(A), B(B), verge_weight(weight) { }; // Конструктор
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
		struct Comparator
		{
			bool operator() (const Ptr &lhs, const Ptr &rhs) const
			{
				return (lhs->A->index < rhs->A->index)
					|| (lhs->A->index == rhs->A->index
						&& lhs->B->index < rhs->B->index);
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
		list<Verge::Ptr> _verges;
		Graph *owner;
	public:
		Verges(Graph *owner) : owner(owner) {};
		~Verges() { _verges.clear(); };
		
		bool Add(int A_idx, int B_idx, double weight = 0) {
			Node::Ptr sp_A = owner->nodes(A_idx);
			Node::Ptr sp_B = owner->nodes(B_idx);
			_verges.push_back(make_shared<Verge>(sp_A, sp_B, weight));
			_verges.sort(Verge::Comparator());
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
				_verges.push_back(sp);
				_verges.sort(Verge::Comparator());
			}
			return sp;
		}


		bool Del(int A_idx, int B_idx) {
			for (auto &it : _verges) {
				if (it->A->index == A_idx && it->B->index == B_idx) {
					_verges.remove(it);
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
				cout << "(" << it->A->index << ")-(" << it->B->index << ")[" << it->verge_weight << "]\t";
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


};



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
	G.nodes.Print();

	G.verges.Add(1, 3, 103);
	G.verges.Add(1, 5, 105);
	G.verges(22, 33)->verge_weight = 333;;
	G.verges.Print();


	// The end
	cout << endl << "\n\nEnter to exit...";
	cin.get();
}

