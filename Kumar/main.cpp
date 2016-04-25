#include "header.h"

const bool DEBUG            = true;
const char* WINDOW_TITLE    = "SDL2 Kumario";

static int display_in_use   = 0; // only using first display

SDL_Window* window          = NULL;
SDL_Surface* screenSurface  = NULL;
SDL_Rect display_bounds;


int getVideoInfo()
{
    int i, display_mode_count;
    SDL_DisplayMode mode;
    Uint32 f;    

    // get available displays and their modes
    SDL_Log("SDL_GetNumVideoDisplays(): %i", SDL_GetNumVideoDisplays());
    display_mode_count = SDL_GetNumDisplayModes(display_in_use);
    if (display_mode_count < 1) {
        SDL_Log("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
        return 1;
    }
    SDL_Log("SDL_GetNumDisplayModes: %i", display_mode_count);
    for (i = 0; i < display_mode_count; ++i) {
        if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0) {
            SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
            return 1;
        }
        f = mode.format;

        SDL_Log("Mode %i\tbpp %i\t%s\t%i x %i", i,
        SDL_BITSPERPIXEL(f), SDL_GetPixelFormatName(f), mode.w, mode.h);
    }

    // get display bounds
    if (SDL_GetDisplayBounds(display_in_use, &display_bounds) == 0) {
        SDL_Log("Display Bounds: x:%d, y:%d, width:%d, height:%d", 
                display_bounds.x, display_bounds.y, display_bounds.w, display_bounds.h);
    } else {
        SDL_Log("SDL_GetDisplayBounds failed: %s", SDL_GetError());
        return 1;
    }

    // get all video drivers info
    int vdrivers_num = SDL_GetNumVideoDrivers();
    SDL_Log("Number of Video Drivers: %d", vdrivers_num);
    for(int i = 0; i < vdrivers_num; ++i) {
        SDL_Log("Video Driver %d: %s", i, SDL_GetVideoDriver(i));
    }

    // get current video driver info
    const char* curr_video_driver = SDL_GetCurrentVideoDriver();
    if( curr_video_driver != NULL ) {
        SDL_Log("Current Video Driver: %s", curr_video_driver);
    } else {
        SDL_Log("SDL_GetCurrentVideoDriver failed: %s", SDL_GetError());
        return 1;
    }

    return 0;
}



int main(/*int argc, char *argv[]*/)
{
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return 1;
    }

    DEBUG && getVideoInfo();


    window = SDL_CreateWindow( WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                display_bounds.w, display_bounds.h, SDL_WINDOW_SHOWN );
    
    if( window != NULL )
    {
        screenSurface = SDL_GetWindowSurface( window );
        // fill surface with color
        SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x99, 0x00 ) );
        // update surface
        SDL_UpdateWindowSurface( window );

        /*
        // set the window opacity
        // This function is available in unstable versions newer than (stable) SDL 2.0.4
        int wop = SDL_SetWindowOpacity(window, 0.5);
        */

        // get window flags
        Uint32 wf = SDL_GetWindowFlags( window );
        SDL_Log("Window Flags: %u", wf);

        /*
        // check Fullscreen changes
        SDL_Delay( 1000 );
        int fullscr = SDL_SetWindowFullscreen( window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        if( fullscr != 0 ) {
            SDL_Log("SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) failed: %s", SDL_GetError());
            return 1;
        }
        SDL_Delay( 1000 );
        // back to windowed mode
        fullscr = SDL_SetWindowFullscreen( window, 0);
        if( fullscr != 0 ) {
            SDL_Log("SDL_SetWindowFullscreen(window, 0) failed: %s", SDL_GetError());
            return 1;
        }
        SDL_Delay( 1000 );
        fullscr = SDL_SetWindowFullscreen( window, SDL_WINDOW_FULLSCREEN);
        if( fullscr != 0 ) {
            SDL_Log("SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) failed: %s", SDL_GetError());
            return 1;
        }
        SDL_Delay( 1000 );
        // back to windowed mode
        fullscr = SDL_SetWindowFullscreen( window, 0);
        if( fullscr != 0 ) {
            SDL_Log("SDL_SetWindowFullscreen(window, 0) failed: %s", SDL_GetError());
            return 1;
        }
        */


        /*
        SDL_MinimizeWindow( window );
        SDL_Delay( 2000 );
        SDL_RestoreWindow( window );
        SDL_Delay( 2000 );
        SDL_MaximizeWindow( window );
        SDL_Delay( 2000 );
        SDL_RestoreWindow( window );
        */


        /*
        SDL_Delay( 2000 );
        SDL_SetWindowBordered( window, SDL_TRUE );
        SDL_Delay( 2000 );
        SDL_SetWindowBordered( window, SDL_FALSE );
        */


        //SDL_EnableScreenSaver();
        // Wait two seconds
        //SDL_Delay( 2000 );
        //SDL_DisableScreenSaver();
    } 
    else {
        SDL_Log( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return 1;
    }


    SDL_Event event;
    bool gameRunning = true;

    while( gameRunning )
    {
        if( SDL_PollEvent(&event) )
        {
            if( event.type == SDL_QUIT ) {
                gameRunning = false;
            }
        } 
    }

    SDL_DestroyWindow( window );
    SDL_Quit();

    return 0;
}
