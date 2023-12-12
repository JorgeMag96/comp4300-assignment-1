#include "assignment.h"

int main(int argc, char* argv[])
{
	YAML::Node config = YAML::LoadFile("resources/config.yaml");

	int			screen_width		= config["screen-width"].as<int>();
	int			screen_height		= config["screen-height"].as<int>();
	int			frame_rate_limit	= config["frame-rate-limit"].as<int>();
	std::string font_file			= config["font-file"].as<std::string>();

	sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "COMP4300 Assignment 1");
	//window.setFramerateLimit(frame_rate_limit);

	// Initialize IMGUI and create a clock used for internal timing
	ImGui::SFML::Init(window);
	sf::Clock delta_clock;

	// Scale the imgui ui by a given factor, does not affect text size
	ImGui::GetStyle().ScaleAllSizes(1.0f);
	ImGui::GetIO().FontGlobalScale = 1.6f;

	// the imgui color {r, g, b } wheel requires floats from 0-1 instead of ints from 0-255
	float black_color[3] = { 0.0f, 0.0f, 0.0f };

	// Load a font so we can display some text
	sf::Font my_font;
	if (!my_font.loadFromFile(font_file)) // attempt to load the font from a file
	{
		exit(EXIT_FAILURE);
	}

	static const char* items[]{ "One","Two","three" };
	static int selected_item = 0;

	bool draw_text = true;
	bool draw_circle = true;

	float c[4] = {1.f, 1.f, 1.f, 1.f};
	float circle_a_radius = 100.0f;
	int circle_a_segments = 30;
	float circle_a_speed_x = 10.0f;
	float circle_a_speed_y = 10.0f;
	sf::CircleShape circle_a(circle_a_radius, circle_a_segments);
	circle_a.setFillColor(sf::Color(c[0] * 255, c[1] * 255, c[2] * 255, c[3] * 255));
	circle_a.setPosition((screen_width / 2) - circle_a.getRadius(), (screen_height / 2) - circle_a.getRadius());
	char circle_a_display_string[255] = "Circle A";
	sf::Text circle_a_text(circle_a_display_string, my_font, 24);

	// main loop - continues for each frame while window is open
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
				exit(EXIT_SUCCESS);
			}

			if (event.type == sf::Event::KeyPressed)
			{
				std::cout << "Key pressed: " << event.key.code << std::endl;
				if (event.key.code == 36) { // Escape key pressed, close window.
					window.close();
					exit(EXIT_SUCCESS);
				}
			}
		}

		window.clear();

		if (draw_circle)
		{
			window.draw(circle_a);
		}

		if (draw_text)
		{
			window.draw(circle_a_text);
		}

		// Draw the ImGUI windows last so it's on top
		ImGui::SFML::Update(window, delta_clock.restart());
		ImGui::Begin("Shape Properties");
		if (ImGui::Combo("Circle", &selected_item, items, IM_ARRAYSIZE(items)))
		{
			std::cout << "Selected: " << selected_item << std::endl;
		}
		ImGui::Checkbox("Draw Circle", &draw_circle);
		ImGui::SameLine();
		ImGui::Checkbox("Draw Text", &draw_text);

		ImGui::SliderFloat("Speed X", &circle_a_speed_x, -500, 500);
		ImGui::SliderFloat("Speed Y", &circle_a_speed_y, -500, 500);
		ImGui::SliderFloat("Radius", &circle_a_radius, 0.0f, 300.0f);
		ImGui::SliderInt("Sides", &circle_a_segments, 3, 64);
		ImGui::ColorEdit4("Color", c);
		
		ImGui::InputText("Text", circle_a_display_string, 255);
		if (ImGui::Button("Set Text"))
		{
			circle_a_text.setString(circle_a_display_string);
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset Circle"))
		{
			circle_a.setPosition((screen_width / 2) - circle_a.getRadius(), (screen_height / 2) - circle_a.getRadius());
		}

		ImGui::End();
		ImGui::SFML::Render(window);

		circle_a.setRadius(circle_a_radius);
		circle_a.setPointCount(circle_a_segments);
		circle_a.setFillColor(sf::Color(c[0] * 255, c[1] * 255, c[2] * 255, c[3] * 255));
		circle_a.setPosition(circle_a.getPosition().x + (circle_a_speed_x * delta_time), circle_a.getPosition().y + (circle_a_speed_y * delta_time));
		circle_a_text.setPosition(
			circle_a.getPosition().x + circle_a.getRadius() - (circle_a_text.getGlobalBounds().width / 2),
			circle_a.getPosition().y + circle_a.getRadius() - (circle_a_text.getGlobalBounds().height)
		);

		// Update the window
		window.display(); 
	}

	return 0;
}
