#include "Entrypoint/Application.h"

#include <spdlog/spdlog.h>
#include <iostream>
#include <queue>
#include <utility>


class DijkstraShortestPathExample : Core::Application
{
	struct Edge {
		char target;
		float weight;
	};

	using Graph = std::unordered_map<char, std::vector<Edge>>;
	using CostNodePair = std::pair<float, char>;

public:
	virtual void Run() override {
		Graph someGraph{
			{'A', { { 'B', 4.0f }, { 'C', 2.0f } } },
			{'B', { { 'C', 1.0f }, { 'D', 5.0f } } },
			{'C', { { 'E', 3.0f } } },
			{'D', { { 'E', 1.0f }, { 'F', 2.0f } } },
			{'E', { { 'F', 5.0f } } },
		};

		DisplayGraph(someGraph);
	};

	std::shared_ptr<std::vector<char>> DijkstraShortestPath(const Graph& graph, char start, char target) {
		// Stores the distance from start node to each node
		std::unordered_map<char, float> dist;
		// Stores where a given node comes from: otherwise known as came-from-map
		std::unordered_map<char, char> parent;
		// Min-heap stores the data in ascending order
		std::priority_queue<CostNodePair, std::vector<CostNodePair>, std::greater<CostNodePair>> pqueue;

		dist[start] = 0.0f;
		pqueue.emplace(0.0f, start);

		while (!pqueue.empty()) {
			auto [currentCost, u] = pqueue.top();

			// Exit early if target is found
			if (u == target)
				break;

			// Where current cost for a node is greater than what the distance map holds for the
			// same node, ignore processing the node again as the smallest distance is already known
			if (currentCost > dist[u])
				continue;

			// Get u's neighbours
			auto foundIter = graph.find(u);
			if (foundIter == graph.end())
				return;

			// Relax u's neighbours
			for (const Edge& e : foundIter->second) {
				const char& v = e.target;
				const float& weight = e.weight;

				float newCost = currentCost + weight;

				// If the neighbour(v) isn't yet in dist map or the new cost to it from start
				// is lesser than what is recorded in the dist map, update the dist map accordingly
				if (dist.find(v) == dist.end() || newCost < dist[v]) {
					// Record distance to v from start
					dist[v] = newCost;
					// Read as v came from u(parent)
					parent[v] = u;
					// Add v to pqueue for processing
					pqueue.emplace(newCost, v);
				}
			}
		}
	}

	void DisplayGraph(const Graph& graph) {
		for (auto& [currentNode, edges] : graph) {
			std::cout << currentNode << ": [ ";

			for (const Edge& e : edges) {
				std::cout << '(' << e.target << ", " << e.weight << ')' << ", ";
			}

			std::cout << "]\n";
		}
	}
};

//DECLARE_MAIN(DijkstraShortestPathExample)