/***********************************************
 * 
 * 	
 * 
 * 
 * 
 * 
 * ********************************************/


#include <string.h> 
#include <time.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

/***** Project Includes *****/

#include "syn_mem_public.h"
#include "syn_mtx_public.h"
#include "syn_sem_public.h"
#include "syn_sys_public.h"
#include "syn_tsk_public.h"

// example compatability
typedef uint8_t bool;
#define true 1
#define false 0
const int SCREEN_FPS = 2;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

uint32_t timer_started = 0;
uint32_t timer_current = 0;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//Current displayed image
SDL_Surface* gCurrentSurface = NULL;

// sdl constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Get window surface
			// gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}

	return success;
}


void close_window()
{
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}


void default_callback(void* packet)
{
	frame_state_reply_packet_t * reply_packet = (frame_state_reply_packet_t *)packet;

	printf("\nRecieved reply:\n"
				   "	frame id: %d\n"
				   "	code: %d\n", reply_packet->data.frame_id, reply_packet->data.code);
}


int main(int argc, char const *argv[]) 
{ 
	// local variables
	int frame_count = 0;

	frame_state_t current_state  = { 0 };
	frame_input_t current_input  = { 0 };

	frame_state_reply_t * current_reply;

	// initialize modules
	packet_init("127.0.0.1", 8080);

	// configure modules
	packet_register_callback(PACKET_FRAME_STATE_REPLY_ID, default_callback);

	// start modules
	packet_start();

	// main loop

	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		// main loop flag
		bool quit = false;

		

		// while application is running
		while( !quit )
		{

			/**** Clear Variables *****/
			memset(&current_state, 0, sizeof(frame_state_t));
			memset(&current_input, 0, sizeof(frame_input_t));
			current_reply = NULL;
			SDL_Event e;
			timer_started = SDL_GetTicks();
			

			/***** Get Input *****/

			while( SDL_PollEvent( &e ) != 0 )
			{
				// user requests quit
				if( e.type == SDL_QUIT )
				{
					quit = true;
				}

				// if mouse event
				if( e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP )
				{
					SDL_GetMouseState( &current_input.x_mouse, &current_input.y_mouse );
				}
			}

			const uint8_t* currentKeyStates = SDL_GetKeyboardState( NULL );
			if( currentKeyStates[ SDL_SCANCODE_UP ] )
			{
				current_input.y_analog = 1.0f;
			}
			if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
			{
				current_input.y_analog = -1.0f;
			}
			if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
			{
				current_input.x_analog = -1.0f;
			}
			if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
			{
				current_input.x_analog = 1.0f;
			}

			// sanitize input
			float analog_magnitude = sqrtf(current_input.x_analog*current_input.x_analog + current_input.y_analog*current_input.y_analog);

			if(analog_magnitude > 0.0f)
			{
				current_input.x_analog = current_input.x_analog/analog_magnitude;
				current_input.y_analog = current_input.y_analog/analog_magnitude;
			}


			/***** Queue New Packets *****/
			frame_state_packet_t current_packet;
			current_packet.packet_id = PACKET_FRAME_STATE_ID;
			current_packet.data = current_state;

			packet_enqueue(&current_packet);


			/***** Update Screen *****/

			SDL_UpdateWindowSurface( gWindow );

			// cap frame rate
			timer_current = SDL_GetTicks() - timer_started;
			if( timer_current < SCREEN_TICKS_PER_FRAME )
			{
				// wait remaining time
				SDL_Delay( SCREEN_TICKS_PER_FRAME - timer_current );
			}
		}
	}

	//Free resources and close SDL
	close_window();

	return 0; 
} 
