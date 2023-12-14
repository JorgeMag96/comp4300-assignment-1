#include "assignment.h"

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
};

struct AssignmentConfig {
	int	screen_width = 1920;
	int screen_height = 1080;
	int frame_rate_limit = 60;
	std::string font_file;
	std::vector<CircleConfig> circles;
	std::vector<RectangleConfig> rectangles;
};

AssignmentConfig config;

double elapsedTimeMillis(std::clock_t c_start)
{
	double time_elapsed_ms = 1000.0 * (std::clock() - c_start) / CLOCKS_PER_SEC;
	return time_elapsed_ms;
}

int main(int argc, char* argv[])
{
	YAML::Node yaml_config = YAML::LoadFile("resources/config.yaml");

	// Parse root settings from YAML
	config.screen_width			= yaml_config["screen-width"].as<int>();
	config.screen_height		= yaml_config["screen-height"].as<int>();
	config.frame_rate_limit		= yaml_config["frame-rate-limit"].as<int>();
	config.font_file			= yaml_config["font-file"].as<std::string>();

	// Load a font so we can display some text
	sf::Font my_font;
	if (!my_font.loadFromFile(config.font_file)) // attempt to load the font from file
	{
		exit(EXIT_FAILURE);
	}

	// Parse shapes from YAML
	std::cout << "Parsing shapes from YAML..." << std::endl;

	auto shapes = yaml_config["shapes"].as<std::vector<YAML::Node>>();
	for (std::size_t i = 0; i < shapes.size(); i++) {
		
		auto shape = shapes[i];
		std::cout << shape << std::endl <<"----------" << std::endl;

		if (shape["type"].as<std::string>() == "Circle")
		{
			CircleConfig circle_config;
			circle_config.name				= shape["name"].as<std::string>();
			circle_config.text				= { shape["display-text"].as<std::string>(), my_font, 24 };
			circle_config.radius			= std::fmax(shape["radius"].as<int>(), 1);
			circle_config.segments			= std::fmax(shape["segments"].as<int>(), 3);
			circle_config.velocity_x		= shape["velocity-x"].as<float>();
			circle_config.velocity_y		= shape["velocity-y"].as<float>();
			auto color_vector				= shape["color"].as<std::vector<int>>();
			if (color_vector.size() != 4) {
				std::cerr << "Error while parsing color property for shape with name= " << circle_config.name << ", array should contain 4 integers. example= [255, 255, 255, 255]" << std::endl;
				exit(EXIT_FAILURE);
			}
			circle_config.color[0]	= std::fmin(std::fmax(color_vector[0], 0), 255) / 255.0f;
			circle_config.color[1]	= std::fmin(std::fmax(color_vector[1], 0), 255) / 255.0f;
			circle_config.color[2]	= std::fmin(std::fmax(color_vector[2], 0), 255) / 255.0f;
			circle_config.color[3]	= std::fmin(std::fmax(color_vector[3], 0), 255) / 255.0f;
			circle_config.draw		= shape["draw"].as<bool>();
			circle_config.draw		= shape["draw"].as<bool>();
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
			rectangle_config.name				= shape["name"].as<std::string>();
			rectangle_config.text				= { shape["display-text"].as<std::string>(), my_font, 24 };
			rectangle_config.width				= std::fmax(shape["width"].as<int>(), 1.0f);
			rectangle_config.height				= std::fmax(shape["height"].as<int>(), 1.0f);
			rectangle_config.velocity_x			= shape["velocity-x"].as<float>();
			rectangle_config.velocity_y			= shape["velocity-y"].as<float>();
			auto color_vector					= shape["color"].as<std::vector<int>>();
			if (color_vector.size() != 4) {
				std::cerr << "Error while parsing color property for shape with name= " << rectangle_config.name << ", array should contain 4 integers. example= [255, 255, 255, 255]" << std::endl;
				exit(EXIT_FAILURE);
			}
			rectangle_config.color[0]	= std::fmin(std::fmax(color_vector[0], 0), 255) / 255.0f;
			rectangle_config.color[1]	= std::fmin(std::fmax(color_vector[1], 0), 255) / 255.0f;
			rectangle_config.color[2]	= std::fmin(std::fmax(color_vector[2], 0), 255) / 255.0f;
			rectangle_config.color[3]	= std::fmin(std::fmax(color_vector[3], 0), 255) / 255.0f;
			rectangle_config.draw		= shape["draw"].as<bool>();
			rectangle_config.draw_text	= shape["draw-text"].as<bool>();

			sf::RectangleShape* rectangle_shape = &rectangle_config.shape;
			rectangle_shape->setSize({ (float)rectangle_config.width, (float)rectangle_config.height });
			rectangle_shape->setPosition(config.screen_width / 2, config.screen_height / 2);
			rectangle_shape->setFillColor(sf::Color(rectangle_config.color[0] * 255, rectangle_config.color[1] * 255, rectangle_config.color[2] * 255, rectangle_config.color[3] * 255));
			config.rectangles.push_back(rectangle_config);
		}
	}

	sf::RenderWindow window(sf::VideoMode(config.screen_width, config.screen_height), "COMP4300 Assignment 1");
	//window.setFramerateLimit(frame_rate_limit);

	// Initialize IMGUI and create a clock used for internal timing
	ImGui::SFML::Init(window);
	sf::Clock delta_clock;

	// Scale the imgui ui by a given factor, does not affect text size
	ImGui::GetStyle().ScaleAllSizes(1.0f);
	ImGui::GetIO().FontGlobalScale = 1.6f;

	CircleConfig*		current_circle_config		= &config.circles[0];
	sf::CircleShape*	current_circle_shape		= &(current_circle_config->shape);
	sf::Text*			current_text				= &(current_circle_config->text);
	std::string			current_display_text		= current_text->getString().toAnsiString();
	float*				current_color				= current_circle_config->color;


	bool pause_simulation = false;
	while (window.isOpen())
	{
		float delta_time = delta_clock.getElapsedTime().asSeconds();
		// Event handling
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(window, event); // pass the event to imgui to be parsed

			if (event.type == sf::Event::Closed)
			{
				window.close();
				// TODO: Save all shapes configurations to config.yaml
				exit(EXIT_SUCCESS);
			}

			if (event.type == sf::Event::KeyPressed)
			{
				std::cout << "Key pressed: " << event.key.code << std::endl;
				if (event.key.code == 36) { // Escape key pressed, close window.
					window.close();
					// TODO: Save all shapes configurations to config.yaml
					exit(EXIT_SUCCESS);
				}
			}
		}

		window.clear();

		ImGui::SFML::Update(window, delta_clock.restart());
		ImGui::Begin("Shape Properties");
		if (ImGui::BeginCombo("Name", current_circle_config->name.c_str()))
		{
			for (size_t i = 0; i < config.circles.size(); i++)
			{
				CircleConfig* circle_config = &config.circles[i];
				bool isSelected = (circle_config == current_circle_config);
				if (ImGui::Selectable(circle_config->name.c_str(), &isSelected))
				{
					current_circle_config	= circle_config;
					current_circle_shape	= &(current_circle_config->shape);
					current_text			= &(current_circle_config->text);
					current_display_text	= current_text->getString().toAnsiString();
					current_color			= current_circle_config->color;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::Checkbox("Draw Shape", &(current_circle_config->draw));
		ImGui::SameLine();
		ImGui::Checkbox("Draw Text", &(current_circle_config->draw_text));
		ImGui::SliderFloat("Velocity X", &(current_circle_config->velocity_x), -500.f, 500.f);
		ImGui::SliderFloat("Velocity Y", &(current_circle_config->velocity_y), -500.f, 500.f);

		if (current_circle_config->shape_type == ShapeTypeEnum::CIRCLE) {
			if (ImGui::SliderInt("Radius", &(current_circle_config->radius), 1, 300))
			{
				current_circle_shape->setRadius(current_circle_config->radius);
			}
			if (ImGui::SliderInt("Sides", &(current_circle_config->segments), 3, 64))
			{
				current_circle_shape->setPointCount(current_circle_config->segments);
			}
		}

		if (current_circle_config->shape_type == ShapeTypeEnum::RECTANGLE) {
			if (ImGui::SliderInt("Width", &(current_circle_config->radius), 1, 300))
			{
				//current_circle_shape->setRadius(current_circle_config->radius);
			}
			if (ImGui::SliderInt("Height", &(current_circle_config->radius), 1, 300))
			{
				//current_circle_shape->setRadius(current_circle_config->radius);
			}
		}
		
		if (ImGui::ColorEdit4("Color", current_color))
		{
			current_circle_shape->setFillColor(sf::Color(current_color[0] * 255, current_color[1] * 255, current_color[2] * 255, current_color[3] * 255));
		}
		
		ImGui::InputText("Text", &current_display_text);
		if (ImGui::Button("Set Text"))
		{
			current_text->setString(current_display_text);
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset Position"))
		{
			current_circle_config->shape.setPosition((window.getSize().x / 2) - current_circle_config->radius, (window.getSize().y / 2) - current_circle_config->radius);
		}
		ImGui::Checkbox("Pause Simulation", &pause_simulation);
		ImGui::End();

		// Draw all circles
		for (auto& circle : config.circles)
		{
			auto* circle_shape_ptr = &circle.shape;
			auto* circle_text_ptr = &circle.text;

			if (!pause_simulation) {
				auto updated_circle_position_x = circle_shape_ptr->getPosition().x + (circle.velocity_x * delta_time);
				auto updated_circle_position_y = circle_shape_ptr->getPosition().y - (circle.velocity_y * delta_time);
				circle_shape_ptr->setPosition(updated_circle_position_x, updated_circle_position_y);

				// Collisions on left/right screen bounds
				if (updated_circle_position_x <= 0 || updated_circle_position_x >= config.screen_width - (circle_shape_ptr->getRadius() * 2))
				{
					if (elapsedTimeMillis(circle.last_collision_clock) > (1000 / config.frame_rate_limit)) // Only change direction if at least one loop tick has elapsed.
					{
						circle.velocity_x *= -1;
						circle.last_collision_clock = std::clock();
					}
				}

				// Collisions on top/bottom screen bounds
				if (updated_circle_position_y <= 0 || updated_circle_position_y + (circle_shape_ptr->getRadius() * 2) >= config.screen_height) 
				{
					if (elapsedTimeMillis(circle.last_collision_clock) > (1000 / config.frame_rate_limit)) // Only change direction if at least one loop tick has elapsed.
					{
						circle.velocity_y *= -1;
						circle.last_collision_clock = std::clock();
					}
				}

				auto updated_text_position_x = circle_shape_ptr->getPosition().x + circle_shape_ptr->getRadius() - (circle_text_ptr->getGlobalBounds().width / 2);
				auto updated_text_position_y = circle_shape_ptr->getPosition().y + circle_shape_ptr->getRadius() - (circle_text_ptr->getGlobalBounds().height);
				circle_text_ptr->setPosition(updated_text_position_x, updated_text_position_y);
			}

			if (circle.draw)
			{
				window.draw(*circle_shape_ptr);
			}

			if (circle.draw_text)
			{
				window.draw(*circle_text_ptr);
			}
		}

		ImGui::SFML::Render(window); // Lastly draw ImGUI so that it shows on top

		window.display(); // Display on screen what has been rendered to the window so far
	}

	return 0;
}
