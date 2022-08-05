#include "../../hdr/graphics/window.hpp"

namespace engine {
	namespace render {
		namespace window {
			const gs::Vec2i windowSize = gs::Vec2i(1920, 1080);

			sf::RenderWindow* winmain = nullptr;
			bool isFullscreen;

			int framerate = 60;
			int virtualFramerate = 60;

			sf::Image logo;

			void create(bool fullscreen) {
				isFullscreen = fullscreen; 

				if (winmain == nullptr) {
					winmain = new sf::RenderWindow(
						sf::VideoMode(windowSize.x, windowSize.y),
						"Procedural Pacman", mGetStyle(isFullscreen)
					);
					logo.loadFromFile(mAssetDirectory + "logo.png"); 
				}
				else {
					winmain->create(
						sf::VideoMode(windowSize.x, windowSize.y),
						"Procedural Pacman", mGetStyle(isFullscreen)
					);
				}

				initStates(); 
			}
			void initStates() {
				gs::input::setWindow(winmain); 

				winmain->setSize(gs::Vec2u(
					sf::VideoMode::getDesktopMode().width,
					sf::VideoMode::getDesktopMode().height
				));

				if (logo.getSize().x > 0)
					winmain->setIcon(
						logo.getSize().x, logo.getSize().y, logo.getPixelsPtr()
					);
			}
			void update() {
				sf::Event action;

				gs::input::updateInputs(); 

				while (winmain->pollEvent(action)) {
					gs::input::updateEvents(action); 

					switch (action.type) {
					case sf::Event::Closed:
						winmain->close(); 
						break; 
					case sf::Event::KeyPressed:
						switch (action.key.code) {
						case sf::Keyboard::F11:
							create(!isFullscreen); 
							break;
						}
						break;
					}
				}
			}
			void close() {
				delete winmain; 
			}

			void begin(gs::Color fillColor) {
				winmain->clear(fillColor); 
			}
			void end() {
				winmain->display(); 
			}
		}
	}
}