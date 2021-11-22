#include <mutex>
#include <SDL2/SDL.h>
#include "backend/map_manager/Map_manager.hpp"
#include "frontend/map_generator/Basic_terrain.hpp"

int main()
{
	Basic_terrain		basic_terrain;
	Map_manager			map_manager(basic_terrain);

	map_manager.init();
	map_manager.lunch();
	bool quit = false;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Event event;

	SDL_Window * window = SDL_CreateWindow("Voxel test",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, 0);
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture * texture = SDL_CreateTexture(renderer,
	    SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 1920, 1080);

	while (!quit)
	{
		SDL_PollEvent(&event);
		std::mutex *mutex;
		//auto pixels = map_manager.get_screen(mutex);
		//std::lock_guard<std::mutex> guard(*mutex);
		//SDL_UpdateTexture(texture, NULL, pixels, 1920 * sizeof(Uint32));

		switch (event.type)
		{
			case SDL_QUIT:
				quit = true;
				break;
		}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
