#pragma once
#include <utility>
#include <SDL2/SDL.h>
/*
 * Recurse through the list of arguments to clean up, cleaning up
 * the first one in the list each iteration.
 */
template<typename T, typename... Args>
void cleanup(T *t, Args&&... args){
	//Cleanup the first item in the list
	cleanup(t);
	//Clean up the remaining arguments
	cleanup(std::forward<Args>(args)...);
}
/*
 * These specializations serve to free the passed argument and also provide the
 * base cases for the recursive call above, eg. when args is only a single
 * item one of the specializations below will be called by
 * cleanup(std::forward<Args>(args)...), ending the recursion
 */
template<>
inline void cleanup<SDL_Window>(SDL_Window *window){
	if (!window){
		return;
	}
	SDL_DestroyWindow(window);
}
template<>
inline void cleanup<SDL_Renderer>(SDL_Renderer *renderer){
	if (!renderer){
		return;
	}
	SDL_DestroyRenderer(renderer);
}
template<>
inline void cleanup<SDL_Texture>(SDL_Texture *texture){
	if (!texture){
		return;
	}
	SDL_DestroyTexture(texture);
}
template<>
inline void cleanup<SDL_Surface>(SDL_Surface *surface){
	if (!surface){
		return;
	}
	SDL_FreeSurface(surface);
}
