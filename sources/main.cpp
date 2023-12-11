#include "assignment.h"

int main(int argc, char* argv[])
{
	YAML::Node config = YAML::LoadFile("resources/config.yaml");

	int			width				= config["width"].as<int>();
	int			height				= config["height"].as<int>();
	int			frame_rate_limit	= config["frame-rate-limit"].as<int>();
	std::string font_file			= config["font-file"].as<std::string>();

	std::cout << "width: " << width << std::endl;
	std::cout << "height: " << height << std::endl;
	std::cout << "frame_rate_limit: " << frame_rate_limit << std::endl;
	std::cout << "font_file: " << font_file << std::endl;

	sf::RenderWindow window(sf::VideoMode(width, height), "COMP4300 Assignment 1");
	window.setFramerateLimit(frame_rate_limit);

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

	bool draw_text = true;
	sf::Text text("Sample Text", my_font, 24);
	
	// position the top-left corner of the text so that the text aligns on the bottom
	// text character size is in pixels, so move the text up from the bottom by its height
	text.setPosition(0, height - (float)text.getCharacterSize());

	char display_string[255] = "Sample Text";

	// main loop - continues for each frame while window is open
	while (window.isOpen())
	{
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

		ImGui::SFML::Update(window, delta_clock.restart());

		ImGui::Begin("Shape Properties");
		ImGui::Text("Window text!");
		ImGui::End();

		window.clear();

		if (draw_text)
		{
			window.draw(text);
		}

		ImGui::SFML::Render(window); // draw the ui last so it's on top

		window.display(); // Update the window
	}

	return 0;
}
