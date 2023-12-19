#include "assignment.h"

double elapsedTimeMillis(std::clock_t c_start)
{
	double time_elapsed_ms = 1000.0 * (std::clock() - c_start) / CLOCKS_PER_SEC;
	return time_elapsed_ms;
}

int main(int argc, char* argv[])
{
	AssignmentConfig config;
	sf::Font font;
	loadConfig(config, font);

	sf::RenderWindow window(sf::VideoMode(config.screen_width, config.screen_height), "COMP4300 Assignment 1");
	window.setFramerateLimit(config.frame_rate_limit);

	// Initialize IMGUI and create a clock used for internal timing
	ImGui::SFML::Init(window);
	sf::Clock delta_clock;

	// Scale the imgui ui by a given factor, does not affect text size
	ImGui::GetStyle().ScaleAllSizes(1.0f);
	ImGui::GetIO().FontGlobalScale = 1.6f;

	void*	current_shape_config = static_cast<void*>(&config.circles[0]);
	ShapeTypeEnum current_shape_type;
	std::string current_display_text = static_cast<CircleConfig*>(current_shape_config)->text.getString().toAnsiString();
	std::memcpy(&current_shape_type, current_shape_config, sizeof(ShapeTypeEnum));

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
				saveConfig(config);
				exit(EXIT_SUCCESS);
			}

			if (event.type == sf::Event::KeyPressed)
			{
				std::cout << "Key pressed: " << event.key.code << std::endl;
				if (event.key.code == 36) { // Escape key pressed, close window.
					window.close();
					saveConfig(config);
					exit(EXIT_SUCCESS);
				}
			}
		}

		window.clear();

		ImGui::SFML::Update(window, delta_clock.restart());
		ImGui::Begin("Shape Properties");

		// Generic combo, first we need to check if the current selection is a circle or rectangle.
		std::string current_shape_name;
		if (current_shape_type == ShapeTypeEnum::CIRCLE)
		{
			current_shape_name = static_cast<CircleConfig*>(current_shape_config)->name;
		}
		else if (current_shape_type == ShapeTypeEnum::RECTANGLE)
		{
			current_shape_name = static_cast<RectangleConfig*>(current_shape_config)->name;
		}

		if (ImGui::BeginCombo("Name", current_shape_name.c_str()))
		{
			for (size_t i = 0; i < config.circles.size(); i++)
			{
				CircleConfig* circle_config = &config.circles[i];
				bool isSelected = (circle_config == static_cast<CircleConfig*>(current_shape_config));
				if (ImGui::Selectable(circle_config->name.c_str(), &isSelected))
				{
					current_shape_config = static_cast<void*>(circle_config);
					std::memcpy(&current_shape_type, current_shape_config, sizeof(ShapeTypeEnum));
					current_display_text = static_cast<CircleConfig*>(current_shape_config)->text.getString().toAnsiString();
				}
			}
			for (size_t i = 0; i < config.rectangles.size(); i++)
			{
				RectangleConfig* rectangle_config = &config.rectangles[i];
				bool isSelected = (rectangle_config == static_cast<RectangleConfig*>(current_shape_config));
				if (ImGui::Selectable(rectangle_config->name.c_str(), &isSelected))
				{
					current_shape_config = static_cast<void*>(rectangle_config);
					std::memcpy(&current_shape_type, current_shape_config, sizeof(ShapeTypeEnum));
					current_display_text = static_cast<RectangleConfig*>(current_shape_config)->text.getString().toAnsiString();
				}
			}
			ImGui::EndCombo();
		}

		// Draw menu for circle
		if (current_shape_type == ShapeTypeEnum::CIRCLE) {
			CircleConfig* current_circle_config = static_cast<CircleConfig*>(current_shape_config);
			ImGui::Checkbox("Draw Shape", &(current_circle_config->draw));
			ImGui::SameLine();
			ImGui::Checkbox("Draw Text", &(current_circle_config->draw_text));
			ImGui::SliderFloat("Velocity X", &(current_circle_config->velocity_x), -500.f, 500.f);
			ImGui::SliderFloat("Velocity Y", &(current_circle_config->velocity_y), -500.f, 500.f);
			if (ImGui::SliderInt("Radius", &(current_circle_config->radius), 1, 300))
			{
				current_circle_config->shape.setRadius(current_circle_config->radius);
			}
			if (ImGui::SliderInt("Sides", &(current_circle_config->segments), 3, 64))
			{
				current_circle_config->shape.setPointCount(current_circle_config->segments);
			}
			if (ImGui::ColorEdit4("Color", current_circle_config->color))
			{
				current_circle_config->shape.setFillColor(sf::Color(
					current_circle_config->color[0] * 255,
					current_circle_config->color[1] * 255,
					current_circle_config->color[2] * 255,
					current_circle_config->color[3] * 255)
				);
			}

			ImGui::InputText("Text", &current_display_text);
			if (ImGui::Button("Set Text"))
			{
				current_circle_config->text.setString(current_display_text);
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset Position"))
			{
				current_circle_config->shape.setPosition((window.getSize().x / 2) - current_circle_config->radius, (window.getSize().y / 2) - current_circle_config->radius);
			}
		}

		// Draw menu for rectangle
		if (current_shape_type == ShapeTypeEnum::RECTANGLE) {
			RectangleConfig* current_rectangle_config = static_cast<RectangleConfig*>(current_shape_config);
			sf::RectangleShape* current_rectangle_shape = &current_rectangle_config->shape;
			ImGui::Checkbox("Draw Shape", &(current_rectangle_config->draw));
			ImGui::SameLine();
			ImGui::Checkbox("Draw Text", &(current_rectangle_config->draw_text));
			ImGui::SliderFloat("Velocity X", &(current_rectangle_config->velocity_x), -500.f, 500.f);
			ImGui::SliderFloat("Velocity Y", &(current_rectangle_config->velocity_y), -500.f, 500.f);
			if (ImGui::SliderInt("Width", &(current_rectangle_config->width), 1, 300))
			{
				current_rectangle_shape->setSize({ (float) current_rectangle_config->width,  (float) current_rectangle_config->height });
			}
			if (ImGui::SliderInt("Height", &(current_rectangle_config->height), 1, 300))
			{
				current_rectangle_shape->setSize({ (float) current_rectangle_config->width,  (float) current_rectangle_config->height });
			}
			
			ImGui::InputText("Text", &current_display_text);
			if (ImGui::Button("Set Text"))
			{
				current_rectangle_config->text.setString(current_display_text);
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset Position"))
			{
				current_rectangle_shape->setPosition((window.getSize().x / 2), (window.getSize().y / 2));
			}
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
				if (updated_circle_position_x <= 0 || updated_circle_position_x >= config.screen_width - (circle.radius * 2))
				{
					if (elapsedTimeMillis(circle.last_collision_clock) > (1000 / config.frame_rate_limit)) // Only change direction if at least one loop tick has elapsed.
					{
						circle.velocity_x *= -1;
						circle.last_collision_clock = std::clock();
					}
				}

				// Collisions on top/bottom screen bounds
				if (updated_circle_position_y <= 0 || updated_circle_position_y + (circle.radius * 2) >= config.screen_height)
				{
					if (elapsedTimeMillis(circle.last_collision_clock) > (1000 / config.frame_rate_limit)) // Only change direction if at least one loop tick has elapsed.
					{
						circle.velocity_y *= -1;
						circle.last_collision_clock = std::clock();
					}
				}

				auto updated_text_position_x = circle_shape_ptr->getPosition().x + circle.radius - (circle_text_ptr->getGlobalBounds().width / 2);
				auto updated_text_position_y = circle_shape_ptr->getPosition().y + circle.radius - (circle_text_ptr->getGlobalBounds().height);
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

		// Draw all rectangles
		for (auto& rectangle : config.rectangles)
		{
			auto* rectangle_shape_ptr = &rectangle.shape;
			auto* rectangle_text_ptr = &rectangle.text;

			if (!pause_simulation) {
				auto updated_rectangle_position_x = rectangle_shape_ptr->getPosition().x + (rectangle.velocity_x * delta_time);
				auto updated_rectangle_position_y = rectangle_shape_ptr->getPosition().y - (rectangle.velocity_y * delta_time);
				rectangle_shape_ptr->setPosition(updated_rectangle_position_x, updated_rectangle_position_y);

				// Collisions on left/right screen bounds
				if (updated_rectangle_position_x <= 0 || updated_rectangle_position_x >= config.screen_width - rectangle.width )
				{
					if (elapsedTimeMillis(rectangle.last_collision_clock) > (1000 / config.frame_rate_limit)) // Only change direction if at least one loop tick has elapsed.
					{
						rectangle.velocity_x *= -1;
						rectangle.last_collision_clock = std::clock();
					}
				}

				// Collisions on top/bottom screen bounds
				if (updated_rectangle_position_y <= 0 || updated_rectangle_position_y + rectangle.height >= config.screen_height)
				{
					if (elapsedTimeMillis(rectangle.last_collision_clock) > (1000 / config.frame_rate_limit)) // Only change direction if at least one loop tick has elapsed.
					{
						rectangle.velocity_y *= -1;
						rectangle.last_collision_clock = std::clock();
					}
				}

				auto updated_text_position_x = rectangle_shape_ptr->getPosition().x + (rectangle.width / 2) - (rectangle_text_ptr->getGlobalBounds().width / 2);
				auto updated_text_position_y = rectangle_shape_ptr->getPosition().y + (rectangle.height / 2 ) - (rectangle_text_ptr->getGlobalBounds().height);
				rectangle_text_ptr->setPosition(updated_text_position_x, updated_text_position_y);
			}

			if (rectangle.draw)
			{
				window.draw(*rectangle_shape_ptr);
			}

			if (rectangle.draw_text)
			{
				window.draw(*rectangle_text_ptr);
			}
		}

		ImGui::SFML::Render(window); // Lastly draw ImGUI so that it shows on top

		window.display(); // Display on screen what has been rendered to the window so far
	}

	return 0;
}
