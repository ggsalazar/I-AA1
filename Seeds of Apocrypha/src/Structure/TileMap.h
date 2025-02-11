#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

class TileMap : public sf::Drawable, public sf::Transformable {
public:
	bool load(const string& tileset, sf::Vector2u tile_size, const string& json_file) {
		//Load json file
		ifstream file(json_file);
		if (!file.is_open()) {
			cerr << "Failed to open " << json_file << "!" << endl;
			return false;
		}

		json tilemap_data;
		file >> tilemap_data;

		int map_w = tilemap_data["width"];
		int map_h = tilemap_data["height"];

		//Get the map data from the first layer
		vector<int> tile_data = tilemap_data["layers"][0]["data"];

		//Load tileset texture
		if (!m_tileset.loadFromFile(tileset)) {
			cerr << "Failed to load tileset " << tileset << "!" << endl;
			return;
		}

		//Resize vertex array
		m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
		m_vertices.resize(map_w * map_h * 6);

		//Populate vertex array
		for (unsigned int x = 0; x < map_w; ++x) {
			for (unsigned int y = 0; y < map_h; ++y) {
				//Get current tile ID number
				const int tile_ID = tile_data[x + y * map_w] - 1; //Is it 1-indexed? or 0-indexed?

				//Skip empty tiles
				if (tile_ID < 0) continue;

				//Find its position in the tileset texture
				const int tu = tile_ID % (m_tileset.getSize().x / tile_size.x);
				const int tv = tile_ID / (m_tileset.getSize().x / tile_size.x);

				sf::Vertex* tri = &m_vertices[(x + y * map_w) * 6];

				tri[0].position = sf::Vector2f(x * tile_size.x, y * tile_size.y);
				tri[1].position = sf::Vector2f((x+1) * tile_size.x, y * tile_size.y);
				tri[2].position = sf::Vector2f(x * tile_size.x, (y+1) * tile_size.y);
				tri[3].position = sf::Vector2f(x * tile_size.x, (y+1) * tile_size.y);
				tri[3].position = sf::Vector2f((x+1) * tile_size.x, y * tile_size.y);
				tri[3].position = sf::Vector2f((x+1) * tile_size.x, (y+1) * tile_size.y);

				tri[0].texCoords = sf::Vector2f(tu * tile_size.x, tv * tile_size.y);
				tri[1].texCoords = sf::Vector2f((tu+1) * tile_size.x, tv * tile_size.y);
				tri[2].texCoords = sf::Vector2f(tu * tile_size.x, (tv+1) * tile_size.y);
				tri[3].texCoords = sf::Vector2f(tu * tile_size.x, (tv+1) * tile_size.y);
				tri[4].texCoords = sf::Vector2f((tu+1) * tile_size.x, tv * tile_size.y);
				tri[5].texCoords = sf::Vector2f((tu+1) * tile_size.x, (tv+1) * tile_size.y);
			}
		}

		return true;
	}

private:
	sf::VertexArray m_vertices;
	sf::Texture m_tileset;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		//Apply the transform
		states.transform *= getTransform();

		//Apply the tileset texture
		states.texture = &m_tileset;

		//Draw the vertex array
		target.draw(m_vertices, states);
	}
};