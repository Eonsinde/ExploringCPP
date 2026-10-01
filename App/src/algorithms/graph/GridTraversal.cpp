#include "EntryPoint/Application.h"
#include "EntryPoint/Helpers/Path.h"
#include "Classes/Utils/Graph.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <utility>
#include <unordered_map>
#include <memory>


class GridTraversal : public Core::Application
{
	using json = nlohmann::json;

public:
	virtual void Run() override {
		std::filesystem::path resourcePath = Core::GetExecutableDirectory() / "resources" / "maze.json";

		std::ifstream ifs(resourcePath);

		if (!ifs.is_open()) {
			spdlog::info("Current Path: {}", resourcePath.string());
			return;
		}

		json data = json::parse(ifs);

		size_t row = data["gridDimensions"]["rows"];
		size_t cols = data["gridDimensions"]["cols"];
		
		Core::Cell startCell = data.at("start");
		Core::Cell targetCell = data.at("end");

		Core::Grid<int>::Grid2D mazeGrid = data["grid"];

		spdlog::info("Start: ({}, {})", startCell.row, startCell.column);
		spdlog::info("Target/Goal: ({}, {})\n", targetCell.row, targetCell.column);

		//Core::Grid<int>::DepthFirstSearch(mazeGrid, startCell, targetCell);
		Core::Grid<int>::BreadthFirstSearch(mazeGrid, startCell, targetCell);
	}
};

//DECLARE_MAIN(GridTraversal)