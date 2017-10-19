#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>  //Fons de pantalla-personatge-moviment
#include <SDL_ttf.h>    //Fonts de text
#include <SDL_mixer.h>
#include <iostream>
using namespace std;

//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


bool in_rect(int x, int y, struct SDL_Rect *r) {
	return (x >= r->x) && (y >= r->y) &&
		(x < r->x + r->w) && (y < r->y + r->h);
}

bool onPlayMusic = false;
bool onStopMusic = false;

int main(int, char*[]) {

	// --- INIT ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw "No es pot inicialitzar SDL subsystems";

	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	if (!(IMG_Init(imgFlags)& imgFlags)) throw "Error: SDL_image init";

	if (TTF_Init() != 0) throw "No es pot inicialitzar SDL_ttf";

	const Uint8 mixFlags{ MIX_INIT_MP3 | MIX_INIT_OGG };
	if (!(Mix_Init(mixFlags)& mixFlags)) throw "Error: SDL_mixer init";
	


	// --- WINDOW ---
	SDL_Window *window{ SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
	if (window == nullptr) throw "No es pot inicialitzar SDL_Window";

	// --- RENDERER ---
	SDL_Renderer *renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
	if (renderer == nullptr) throw "No es pot inicialitzar SDL_Renderer";

	// --- SPRITES --- (Textures)

		//BG -- (Fons de pantalla)
	SDL_Texture *bgTexture{ IMG_LoadTexture(renderer,"../../res/img/bg.jpg") };
	if (bgTexture == nullptr) throw "No s'han pogut crear les textures";
	SDL_Rect bgRect{ 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };

		//Player
	SDL_Texture *playerTexture{ IMG_LoadTexture(renderer,"../../res/img/kintoun.png") };
	if (playerTexture == nullptr) throw "No s'han pogut crear les textures";
	SDL_Rect playerRect{ 0,0, 350/3, 189/3 };
	SDL_Rect playerTarget{ 0,0,100,100 };

		// --- Animated Sprite --- (DIJOUS)

	// --- TEXT ---

		//Titol
	TTF_Font *font{ TTF_OpenFont("../../res/ttf/saiyan.ttf",80) };
	if (font == nullptr) throw "No es pot inicialitzar the TTF_Font";
	SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font,"My first SDL game", SDL_Color{255,150,0,255}) };
	if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
	SDL_Texture *textTexture{ SDL_CreateTextureFromSurface(renderer,tmpSurf) };
	SDL_Rect textRect{ 100,50,tmpSurf->w, tmpSurf->h };
	SDL_FreeSurface(tmpSurf);
	TTF_CloseFont(font);

		//PlayMusic1
	TTF_Font *font2{ TTF_OpenFont("../../res/ttf/saiyan.ttf", 60) };
	if (font == nullptr) throw "No es pot inicialitzar the TTF_Font";
	SDL_Surface *tmpSurf2{ TTF_RenderText_Blended(font2,"Play Music", SDL_Color{ 35,250,0,255 }) };
	if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
	SDL_Texture *playMusicTexture{ SDL_CreateTextureFromSurface(renderer, tmpSurf2) };
	SDL_Rect textPlayMusicRect{ 260,275,tmpSurf2->w, tmpSurf2->h};
	SDL_FreeSurface(tmpSurf2);
		//PlayMusic2
	SDL_Surface *tmpSurf5{ TTF_RenderText_Blended(font2,"Play Music", SDL_Color{ 0,0,255,255 }) };
	if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
	SDL_Texture *playMusicTexture2{ SDL_CreateTextureFromSurface(renderer, tmpSurf5) };
	SDL_Rect textPlayMusicRect2{ 260,275,tmpSurf5->w, tmpSurf5->h };
	SDL_FreeSurface(tmpSurf5);

		//StopMusic
	SDL_Surface *tmpSurf3{ TTF_RenderText_Blended(font2,"Stop Music", SDL_Color{ 255,15,0,255 }) };
	if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
	SDL_Texture *stopMusicTexture{ SDL_CreateTextureFromSurface(renderer, tmpSurf3) };
	SDL_Rect textStopMusicRect{ 260,350,tmpSurf3->w, tmpSurf3->h };
	SDL_FreeSurface(tmpSurf2);
		//StopMusic2
	SDL_Surface *tmpSurf4{ TTF_RenderText_Blended(font2,"Stop Music", SDL_Color{ 255,255,0,255 }) };
	if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
	SDL_Texture *stopMusicTexture2{ SDL_CreateTextureFromSurface(renderer, tmpSurf4) };
	SDL_Rect textStopMusicRect2{ 260,350,tmpSurf4->w, tmpSurf4->h };
	SDL_FreeSurface(tmpSurf4);
	TTF_CloseFont(font);

	// --- AUDIO ---

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		throw "Unable to initialize SDL_mixer audio system";
	}
	Mix_Music *soundtrack{ Mix_LoadMUS("../../res/au/mainTheme.mp3") };
	if (!soundtrack) throw "Unable to load the Mix_Music soundtrack";
	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
	

	// --- GAME LOOP ---
	SDL_Event event;
	bool isRunning = true;
	while (isRunning){
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:		isRunning = false; break;
			case SDL_KEYDOWN:	if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false; break;
			case SDL_MOUSEMOTION: //playerRect.x = event.motion.x - playerRect.w/2; playerRect.y = event.motion.y - playerRect.h/2;
				playerTarget.x = event.motion.x - playerRect.w / 2; playerTarget.y = event.motion.y - playerRect.h / 2; break;
			case SDL_MOUSEBUTTONDOWN: if (in_rect(playerRect.x,playerRect.y, &textPlayMusicRect) == true) {
										Mix_PlayMusic(soundtrack, -1);
										} 
									  if (in_rect(playerRect.x, playerRect.y, &textStopMusicRect) == true) {
										  Mix_PauseMusic();
									  }
									  
									  break;
			default:;
			}
		}

		// UPDATE
			//Interpolaciˇ vector moviment -- SMOOTH
		playerRect.x += (playerTarget.x - playerRect.x) / 6;
		playerRect.y += (playerTarget.y - playerRect.y) / 6;
		
		if (in_rect(playerRect.x, playerRect.y, &textPlayMusicRect) == true) {
			onPlayMusic = true;
		}
		else {
			onPlayMusic = false;
		}
		if (in_rect(playerRect.x, playerRect.y, &textStopMusicRect) == true) {
			onStopMusic = true;
		}
		else {
			onStopMusic = false;
		}

		// DRAW
		SDL_RenderClear(renderer);
			//Background
		
			//Animated Sprite

		SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect);
		SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
		if (onPlayMusic) {
			SDL_RenderCopy(renderer, playMusicTexture2, nullptr, &textPlayMusicRect2);
		}
		else {
			SDL_RenderCopy(renderer, playMusicTexture, nullptr, &textPlayMusicRect);
		}
		if (onStopMusic) {
			SDL_RenderCopy(renderer, stopMusicTexture2, nullptr, &textStopMusicRect2);
		}
		else {
			SDL_RenderCopy(renderer, stopMusicTexture, nullptr, &textStopMusicRect);
		}
		SDL_RenderCopy(renderer, playerTexture, nullptr, &playerRect);
		SDL_RenderPresent(renderer);

	}

	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(playMusicTexture);
	SDL_DestroyTexture(playMusicTexture2);
	SDL_DestroyTexture(stopMusicTexture);
	SDL_DestroyTexture(stopMusicTexture2);
	SDL_DestroyTexture(playerTexture);	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	Mix_CloseAudio(); 

	// --- QUIT ---
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
	return 0;
}