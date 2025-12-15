#include <iostream>

#include <SDL3/SDL.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

void loadRenderer();

SDL_Window* window = NULL;
void createWindow() {
	window = SDL_CreateWindow("Mini Game", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	loadRenderer();
}

SDL_Renderer* renderer = NULL;
void loadRenderer() {
	renderer = SDL_CreateRenderer(window, NULL);
}

void movePlayer(SDL_FRect* rect) {	
	float spd = 0.5f;
	
	const bool *ke_s = SDL_GetKeyboardState(NULL);
	
	if (ke_s[SDL_SCANCODE_A]) {
		rect->x -= spd;
	}
	if (ke_s[SDL_SCANCODE_D]) {
		rect->x += spd;
	}
	if (ke_s[SDL_SCANCODE_W]) {
		rect->y -= spd;
	}
	if (ke_s[SDL_SCANCODE_S]) {
		rect->y += spd;
	}
}

void moveTowards(SDL_FRect* enRect, float x, float y) {
	int hor = (x > enRect->x) ? 1 : -1;
	int ver = (y > enRect->y) ? 1 : -1;

	float spd = 0.1f;

	enRect->x += spd * hor;
	enRect->y += spd * ver;
}

bool checkCollission(SDL_FRect* rect, SDL_FRect* enRect) {
	return ((rect->x) - ((rect->w) / 2) < ((enRect->x) + ((enRect->w) / 2)) && 
			(rect->x) + ((rect->w) / 2) > ((enRect->x) - ((enRect->w) / 2)) &&
			(rect->y) - ((rect->h) / 2) < ((enRect->y) + ((enRect->h) / 2)) &&
			(rect->y) + ((rect->h) / 2) > ((enRect->y) - ((enRect->h) / 2)));
}

int main(void) {
	std::cout << "\033[2J";
	createWindow();
	
	SDL_FRect rect = {
		900.0f, 600.0f,
		100.0f, 100.0f
	};
	
	SDL_FRect enRect = {
		100.0f, 100.0f,
		100.0f, 100.0f
	};
	
	Uint64 startTick = SDL_GetTicks();
	Uint64 lastTick = startTick;

	int secondsPassed = 0;
	int lastSecond = -1;

	bool running = true;
	while (running) {
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_EVENT_QUIT) {
				std::cout << "Fechando jogo...\n";
				running = false;
			}
		}
		movePlayer(&rect);
		moveTowards(&enRect, rect.x, rect.y);
		
		bool colliding = checkCollission(&rect, &enRect);
		if (colliding) {
			running = false;
		}

		Uint64 now = SDL_GetTicks();
		Uint64 delta = now - lastTick;
		lastTick = now;
		
		secondsPassed = (now - startTick) / 1000;
		
		if (secondsPassed != lastSecond) {
			lastSecond = secondsPassed;
			std::cout << "\033[1;1H";
			std::cout << "Tempo sobrevivido: " << secondsPassed << "s.\n\n";
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &rect);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &enRect);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
