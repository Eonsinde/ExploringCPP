#include "EntryPoint/Application.h"

#include <spdlog/spdlog.h>
#include <vector>
#include <unordered_map>
#include <queue>
#include <unordered_set>


// This example gives an introduction to working with the graph
// data structure
class GraphIntroduction : public Core::Application
{
public:
	virtual void Run() override {
		std::cout << "========== Depth First Search ==========\n";

		std::unordered_map<int, std::vector<int>> worldGraph {
			{ 0, { 1, 3, 2, 4 } },
			{ 1, { 5, 3, 0 } },
			{ 2, { 3, 0, 4 } },
			{ 3, { 1, 0, 2 } },
			{ 4, { 0, 2 } },
			{ 5, { 1 } },
		};

		DepthFirstSearch(0, worldGraph.size(), worldGraph);

		
		std::cout << "========== Breadth First Search ==========\n";
		// Example graph as an adjacency list:
		//     0
		//    / \
		//   1   2
		//  / \   \
		// 3   4   5

		std::unordered_map<int, std::vector<int>> simpleTree{
			{ 0, { 1, 2 }},
			{ 1, { 0, 3, 4 } },
			{ 2, { 0, 5 } },
			{ 3, { 1 } },
			{ 4, { 1 } },
			{ 5, { 2 } }
		};
	}

	void DepthFirstSearch(int startNode, int totalNodes, const std::unordered_map<int, std::vector<int>>& graph) {
		std::vector<bool> visited(totalNodes, false);
		std::vector<int> stack;

		stack.push_back(startNode);

		while (!stack.empty()) {
			int current = stack.back();
			stack.pop_back();

			if (!visited[current]) {
				visited[current] = true;

				// Process current node
				spdlog::info("Node {} is visited", current);

				// Traverse through the adjacency list of currentNode
				for (int neighbour : graph.at(current)) {
					if (!visited[neighbour]) {
						stack.push_back(neighbour);
					}
				}
			}
		}
	}

	void BreadthFirstSearch(int startNode, const std::unordered_map<int, std::vector<int>>& graph) {
		std::queue<int> nodeQueue;
		std::unordered_set<int> visited;

		nodeQueue.push(startNode);
		visited.insert(startNode);

		while (!nodeQueue.empty()) {
			int current = nodeQueue.front();
			nodeQueue.pop();

			// Process current node
			spdlog::info("Node {} is visited", current);

			// Process node in the adjacency list of currentNode
			for (int neighbour : graph.at(current)) {
				// If neighbour hasn't been visited
				if (visited.find(neighbour) == visited.end()) {
					visited.insert(neighbour);
					nodeQueue.push(neighbour);
				}
			}
		}
	}
};

//DECLARE_MAIN(GraphIntroduction)
