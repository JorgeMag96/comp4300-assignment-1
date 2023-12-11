#include "comp4300-assignment-1.h"

int main(int argc, char* argv[])
{
	int			width = 1920;
	int			height = 1080;
	int			frame_rate_limit = 60;
	std::string font_file = "resources/fonts/pixelated_arial_bold_11.ttf";

	sf::RenderWindow window(sf::VideoMode(width, height), "COMP4300 Assignment 1");
	window.setFramerateLimit(frame_rate_limit);

	// Initialize IMGUI and create a clock used for internal timing
	ImGui::SFML::Init(window);
	sf::Clock delta_clock;

	// Scale the imgui ui by a given factor, does not affect text size
	ImGui::GetStyle().ScaleAllSizes(1.0f);

	// the imgui color {r, g, b } wheel requires floats from 0-1 instead of ints from 0-255
	float black_color[3] = { 0.0f, 0.0f, 0.0f };

	// let's load a font so we can display some text
	sf::Font my_font;

	// attempt to load the font from a file
	if (!my_font.loadFromFile(font_file))
	{
		exit(-1);
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
			// pass the event to imgui to be parsed
			ImGui::SFML::ProcessEvent(window, event);

			if (event.type == sf::Event::Closed)
			{
				window.close();
				exit(EXIT_SUCCESS);
			}

			if (event.type == sf::Event::KeyPressed)
			{
				std::cout << "Key pressed with code = " << event.key.code << std::endl;

				if (event.key.code == 36) { // Escape key pressed, close window.
					window.close();
					exit(EXIT_SUCCESS);
				}
			}
		}

		ImGui::SFML::Update(window, delta_clock.restart());

		ImGui::Begin("Window title");
		ImGui::Text("Window text!");
		ImGui::End();

		window.clear();

		if (draw_text)
		{
			window.draw(text);
		}

		ImGui::SFML::Render(window); // draw the ui last so it's on top
		window.display();
	}

	return 0;
}
