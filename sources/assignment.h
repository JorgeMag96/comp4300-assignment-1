// comp4300-assignment-1.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <memory>
#include <fstream>
#include <math.h>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui-SFML.h>
#include <yaml-cpp/yaml.h>

enum ShapeTypeEnum
{
	CIRCLE,
	RECTANGLE
};

struct CircleConfig {
	const ShapeTypeEnum shape_type = CIRCLE;
	std::string		name;
	sf::CircleShape	shape;
	sf::Text		text;
	int				radius = 10;
	int				segments = 30;
	float			velocity_x = 10.f;
	float			velocity_y = 10.f;
	float			color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	bool			draw = true;
	bool			draw_text = true;
	std::clock_t	last_collision_clock = std::clock();
};

struct RectangleConfig {
	const ShapeTypeEnum shape_type = RECTANGLE;
	std::string			name;
	sf::RectangleShape	shape;
	sf::Text			text;
	int					width = 60;
	int					height = 20;
	float				velocity_x = 10.f;
	float				velocity_y = 10.f;
	float				color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	bool				draw = true;
	bool				draw_text = true;
	std::clock_t		last_collision_clock = std::clock();
};

struct AssignmentConfig {
	int	screen_width = 1920;
	int screen_height = 1080;
	int frame_rate_limit = 60;
	std::string font_file;
	std::vector<CircleConfig> circles;
	std::vector<RectangleConfig> rectangles;
};

void loadConfig(AssignmentConfig& config, sf::Font& font)
{
	YAML::Node yaml_config = YAML::LoadFile("resources/config.yaml");

	// Parse root settings from YAML
	config.screen_width = yaml_config["screen-width"].as<int>();
	config.screen_height = yaml_config["screen-height"].as<int>();
	config.frame_rate_limit = yaml_config["frame-rate-limit"].as<int>();
	config.font_file = yaml_config["font-file"].as<std::string>();

	// Load a font so we can display some text
	if (!font.loadFromFile(config.font_file)) // attempt to load the font from file
	{
		exit(EXIT_FAILURE);
	}

	// Parse shapes from YAML
	std::cout << "Parsing shapes from YAML..." << std::endl;

	auto shapes = yaml_config["shapes"].as<std::vector<YAML::Node>>();
	for (std::size_t i = 0; i < shapes.size(); i++) {

		auto shape = shapes[i];
		std::cout << shape << std::endl << "----------" << std::endl;

		if (shape["type"].as<std::string>() == "Circle")
		{
			CircleConfig circle_config;
			circle_config.name = shape["name"].as<std::string>();
			circle_config.text = { shape["display-text"].as<std::string>(), font, 24 };
			circle_config.radius = std::fmax(shape["radius"].as<int>(), 1);
			circle_config.segments = std::fmax(shape["segments"].as<int>(), 3);
			circle_config.velocity_x = shape["velocity-x"].as<float>();
			circle_config.velocity_y = shape["velocity-y"].as<float>();
			auto color_vector = shape["color"].as<std::vector<int>>();
			if (color_vector.size() != 4) {
				std::cerr << "Error while parsing color property for shape with name= " << circle_config.name << ", array should contain 4 integers. example= [255, 255, 255, 255]" << std::endl;
				exit(EXIT_FAILURE);
			}
			circle_config.color[0] = std::fmin(std::fmax(color_vector[0], 0), 255) / 255.0f;
			circle_config.color[1] = std::fmin(std::fmax(color_vector[1], 0), 255) / 255.0f;
			circle_config.color[2] = std::fmin(std::fmax(color_vector[2], 0), 255) / 255.0f;
			circle_config.color[3] = std::fmin(std::fmax(color_vector[3], 0), 255) / 255.0f;
			circle_config.draw = shape["draw"].as<bool>();
			circle_config.draw = shape["draw"].as<bool>();
			circle_config.draw_text = shape["draw-text"].as<bool>();

			sf::CircleShape* circle_shape = &circle_config.shape;
			circle_shape->setRadius(circle_config.radius);
			circle_shape->setPointCount(circle_config.segments);
			circle_shape->setPosition(config.screen_width / 2, config.screen_height / 2);
			circle_shape->setFillColor(sf::Color(circle_config.color[0] * 255, circle_config.color[1] * 255, circle_config.color[2] * 255, circle_config.color[3] * 255));
			config.circles.push_back(circle_config);
		}
		else if (shape["type"].as<std::string>() == "Rectangle")
		{
			RectangleConfig rectangle_config;
			rectangle_config.name = shape["name"].as<std::string>();
			rectangle_config.text = { shape["display-text"].as<std::string>(), font, 24 };
			rectangle_config.width = std::fmax(shape["width"].as<int>(), 1.0f);
			rectangle_config.height = std::fmax(shape["height"].as<int>(), 1.0f);
			rectangle_config.velocity_x = shape["velocity-x"].as<float>();
			rectangle_config.velocity_y = shape["velocity-y"].as<float>();
			auto color_vector = shape["color"].as<std::vector<int>>();
			if (color_vector.size() != 4) {
				std::cerr << "Error while parsing color property for shape with name= " << rectangle_config.name << ", array should contain 4 integers. example= [255, 255, 255, 255]" << std::endl;
				exit(EXIT_FAILURE);
			}
			rectangle_config.color[0] = std::fmin(std::fmax(color_vector[0], 0), 255) / 255.0f;
			rectangle_config.color[1] = std::fmin(std::fmax(color_vector[1], 0), 255) / 255.0f;
			rectangle_config.color[2] = std::fmin(std::fmax(color_vector[2], 0), 255) / 255.0f;
			rectangle_config.color[3] = std::fmin(std::fmax(color_vector[3], 0), 255) / 255.0f;
			rectangle_config.draw = shape["draw"].as<bool>();
			rectangle_config.draw_text = shape["draw-text"].as<bool>();

			sf::RectangleShape* rectangle_shape = &rectangle_config.shape;
			rectangle_shape->setSize({ (float)rectangle_config.width, (float)rectangle_config.height });
			rectangle_shape->setPosition(config.screen_width / 2, config.screen_height / 2);
			rectangle_shape->setFillColor(sf::Color(rectangle_config.color[0] * 255, rectangle_config.color[1] * 255, rectangle_config.color[2] * 255, rectangle_config.color[3] * 255));
			config.rectangles.push_back(rectangle_config);
		}
	}
}

void saveConfig(AssignmentConfig& config)
{
	std::ofstream yaml_ofstream("resources/config.yaml");
	YAML::Emitter yaml_emitter;
	yaml_emitter.SetIndent(2);

	yaml_emitter << YAML::BeginMap;
	yaml_emitter << YAML::Key << "screen-width";
	yaml_emitter << YAML::Value << config.screen_width;
	yaml_emitter << YAML::Key << "screen-height";
	yaml_emitter << YAML::Value << config.screen_height;
	yaml_emitter << YAML::Key << "frame-rate-limit";
	yaml_emitter << YAML::Value << config.frame_rate_limit;
	yaml_emitter << YAML::Key << "font-file";
	yaml_emitter << YAML::Value << config.font_file;
	yaml_emitter << YAML::EndMap;
	yaml_emitter << YAML::Newline;

	yaml_emitter << YAML::BeginMap;
	yaml_emitter << YAML::Key << "shapes";
	yaml_emitter << YAML::Value << YAML::BeginSeq;

	// Save shapes config to YAML
	for (auto& circle : config.circles)
	{
		yaml_emitter << YAML::BeginMap;
		yaml_emitter << YAML::Key << "name";
		yaml_emitter << YAML::Value << circle.name;
		yaml_emitter << YAML::Key << "type";
		yaml_emitter << YAML::Value << (circle.shape_type == ShapeTypeEnum::CIRCLE ? "Circle" : "Rectangle");
		yaml_emitter << YAML::Key << "display-text";
		yaml_emitter << YAML::Value << circle.text.getString();
		yaml_emitter << YAML::Key << "radius";
		yaml_emitter << YAML::Value << circle.radius;
		yaml_emitter << YAML::Key << "segments";
		yaml_emitter << YAML::Value << circle.segments;
		yaml_emitter << YAML::Key << "velocity-x";
		yaml_emitter << YAML::Value << circle.velocity_x;
		yaml_emitter << YAML::Key << "velocity-y";
		yaml_emitter << YAML::Value << circle.velocity_y;
		yaml_emitter << YAML::Key << "color";
		yaml_emitter << YAML::Value << YAML::Flow << YAML::BeginSeq << (int)(circle.color[0] * 255) << (int)(circle.color[1] * 255) << (int)(circle.color[2] * 255) << (int)(circle.color[3] * 255) << YAML::EndSeq;
		yaml_emitter << YAML::Key << "draw";
		yaml_emitter << YAML::Value << circle.draw;
		yaml_emitter << YAML::Key << "draw-text";
		yaml_emitter << YAML::Value << circle.draw_text;
		yaml_emitter << YAML::EndMap;
	}

	for (auto& rectangle : config.rectangles)
	{
		yaml_emitter << YAML::BeginMap;
		yaml_emitter << YAML::Key << "name";
		yaml_emitter << YAML::Value << rectangle.name;
		yaml_emitter << YAML::Key << "type";
		yaml_emitter << YAML::Value << (rectangle.shape_type == ShapeTypeEnum::RECTANGLE ? "Rectangle" : "Circle");
		yaml_emitter << YAML::Key << "display-text";
		yaml_emitter << YAML::Value << rectangle.text.getString();
		yaml_emitter << YAML::Key << "width";
		yaml_emitter << YAML::Value << rectangle.width;
		yaml_emitter << YAML::Key << "height";
		yaml_emitter << YAML::Value << rectangle.height;
		yaml_emitter << YAML::Key << "velocity-x";
		yaml_emitter << YAML::Value << rectangle.velocity_x;
		yaml_emitter << YAML::Key << "velocity-y";
		yaml_emitter << YAML::Value << rectangle.velocity_y;
		yaml_emitter << YAML::Key << "color";
		yaml_emitter << YAML::Value << YAML::Flow << YAML::BeginSeq << (int)(rectangle.color[0] * 255) << (int)(rectangle.color[1] * 255) << (int)(rectangle.color[2] * 255) << (int)(rectangle.color[3] * 255) << YAML::EndSeq;
		yaml_emitter << YAML::Key << "draw";
		yaml_emitter << YAML::Value << rectangle.draw;
		yaml_emitter << YAML::Key << "draw-text";
		yaml_emitter << YAML::Value << rectangle.draw_text;
		yaml_emitter << YAML::EndMap;
	}

	yaml_emitter << YAML::EndSeq;
	yaml_emitter << YAML::EndMap;
	yaml_emitter << YAML::Newline;

	yaml_ofstream << yaml_emitter.c_str();
}
