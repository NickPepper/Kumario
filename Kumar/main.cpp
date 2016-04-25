#include "header.h"

const bool DEBUG                = true;
const char* WINDOW_TITLE        = "SDL2 Kumario";
//const char* splashImageFileName = "assets/images/Ganja.gif";//TODO: animated GIF is not supported by default
//const char* splashImageFileName = "assets/images/rastafarian_dancing.gif";//TODO: animated GIF is not supported by default
const char* splashImageFileName = "assets/images/matrix-has-you.jpg";

static int display_in_use   = 0; // only using first display

SDL_Window* gWindow         = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gSplashImage   = NULL;
SDL_Rect display_bounds;

// music that will be played on splash screen
Mix_Music *gMusic = NULL;
const char* splashAudioFileName = "assets/sounds/butterfly_flew_away.wav";

/*
// sound effects that will be used
Mix_Chunk *gScratch = NULL;
const char* scratchEffectFileName = "assets/sounds/.wav";
Mix_Chunk *gHigh = NULL;
const char* highEffectFileName = "assets/sounds/.wav";
Mix_Chunk *gMedium = NULL;
const char* mediumEffectFileName = "assets/sounds/.wav";
Mix_Chunk *gLow = NULL;
const char* lowEffectFileName = "assets/sounds/.wav";
*/

bool getPlatformAndCPUInformation()
{
    printf("\n");
    
    /*
    If the correct platform name is not available, returns a string beginning with the text "Unknown".
    Here are the names returned for some supported platforms:
        Windows
        Mac OS X
        Linux
        iOS
        Android
    */
    SDL_Log("Platform: %s", SDL_GetPlatform());

    // RAM amount
    SDL_Log("RAM: %d MB", SDL_GetSystemRAM());

    ///////////////////////////////////////////////////////////
    // TODO: OS name (!)
    // TODO: processor model (!)
    // TODO: byte order (!)
    // TODO: etc info (!)
    ///////////////////////////////////////////////////////////

    // number of logical cores
    SDL_Log("Logical CPU cores: %d", SDL_GetCPUCount());

    // useful for determining multi-threaded structure padding or SIMD prefetch sizes
    SDL_Log("CPU's L1 cache line size: %d KB", SDL_GetCPUCacheLineSize());

    // 3DNow! features support ?
    SDL_Log("3DNow! support: %s", SDL_Has3DNow()?"YEP":"NOPE");

    // AVX features support ?
    SDL_Log("AVX support: %s", SDL_HasAVX()?"YEP":"NOPE");

    // AVX2 features support ?
    SDL_Log("AVX2 support: %s", SDL_HasAVX2()?"YEP":"NOPE");

    // AltiVec features support ?
    SDL_Log("AltiVec support: %s", SDL_HasAltiVec()?"YEP":"NOPE");

    // MMX features support ?
    SDL_Log("MMX support: %s", SDL_HasMMX()?"YEP":"NOPE");

    // RDTSC instruction support?
    SDL_Log("RDTSC support: %s", SDL_HasRDTSC()?"YEP":"NOPE");

    // SSE features support?
    SDL_Log("SSE support: %s", SDL_HasSSE()?"YEP":"NOPE");

    // SSE2 features support?
    SDL_Log("SSE2 support: %s", SDL_HasSSE2()?"YEP":"NOPE");

    // SSE3 features support?
    SDL_Log("SSE3 support: %s", SDL_HasSSE3()?"YEP":"NOPE");

    // SSE4.1 features support?
    SDL_Log("SSE41 support: %s", SDL_HasSSE41()?"YEP":"NOPE");

    // SSE4.2 features support?
    SDL_Log("SSE42 support: %s", SDL_HasSSE42()?"YEP":"NOPE");

    return true;
}


bool getAllVideoDriversInfo()
{
    printf("\n");
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
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "SDL_GetCurrentVideoDriver failed: %s", SDL_GetError() );
        return false;
    }
    return true;
}


bool getAvailableDisplaysAndTheirModes()
{
    bool success = true;
    int display_mode_count;
    SDL_DisplayMode mode;
    Uint32 f;

    printf("\n");

    SDL_Log("SDL_GetNumVideoDisplays(): %i", SDL_GetNumVideoDisplays());
    display_mode_count = SDL_GetNumDisplayModes(display_in_use);
    if (display_mode_count < 1) {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "SDL_GetNumDisplayModes failed: %s", SDL_GetError() );
        success = false;
    }
    SDL_Log("SDL_GetNumDisplayModes: %i", display_mode_count);
    for (int i = 0; i < display_mode_count; ++i) {
        if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0) {
            SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "SDL_GetDisplayMode failed: %s", SDL_GetError() );
            success = false;
        }
        f = mode.format;

        SDL_Log("Mode %i\tbpp %i\t%s\t%i x %i", 
                    i, SDL_BITSPERPIXEL(f), SDL_GetPixelFormatName(f), mode.w, mode.h);
    }

    return success;
}


bool getWindowFlags()
{
    printf("\n");
    Uint32 wf = SDL_GetWindowFlags( gWindow );
    SDL_Log("Window Flags: %u", wf);
    return true;
}


bool getDisplayBounds()
{
    printf("\n");
    if (SDL_GetDisplayBounds(display_in_use, &display_bounds) == 0) {
        SDL_Log("Current Display Bounds: x:%d, y:%d, width:%d, height:%d", 
                    display_bounds.x, display_bounds.y, display_bounds.w, display_bounds.h);
    } else {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "SDL_GetDisplayBounds failed: %s", SDL_GetError() );
        return false;
    }
    return true;
}


bool init()
{
    bool success = true;
    // initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO ) < 0 )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        if( !getDisplayBounds() ) {
            success = false;
        }

        // create window
        gWindow = SDL_CreateWindow( WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                display_bounds.w, display_bounds.h, SDL_WINDOW_SHOWN );
        if( gWindow != NULL )
        {
            // get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
            // fill surface with color
            SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0x00, 0x66, 0x00 ) );
            // update surface
            SDL_UpdateWindowSurface( gWindow );

            /*
            // set the window opacity
            // This function is available in unstable versions newer than (stable) SDL 2.0.4
            int wop = SDL_SetWindowOpacity( gWindow, 0.5 );
            */

            /*
            // check Fullscreen changes
            SDL_Delay( 1000 );
            int fullscr = SDL_SetWindowFullscreen( gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP );
            if( fullscr != 0 ) {
                SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "SDL_SetWindowFullscreen with SDL_WINDOW_FULLSCREEN_DESKTOP failed: %s", SDL_GetError() );
                success = false;
            }
            SDL_Delay( 1000 );
            // back to windowed mode
            fullscr = SDL_SetWindowFullscreen( gWindow, 0 );
            if( fullscr != 0 ) {
                SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "SDL_SetWindowFullscreen with 0 failed: %s", SDL_GetError() );
                success = false;
            }
            SDL_Delay( 1000 );
            fullscr = SDL_SetWindowFullscreen( gWindow, SDL_WINDOW_FULLSCREEN );
            if( fullscr != 0 ) {
                SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "SDL_SetWindowFullscreen with SDL_WINDOW_FULLSCREEN failed: %s", SDL_GetError() );
                success = false;
            }
            SDL_Delay( 1000 );
            // back to windowed mode
            fullscr = SDL_SetWindowFullscreen( gWindow, 0);
            if( fullscr != 0 ) {
                SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "SDL_SetWindowFullscreen with 0 failed: %s", SDL_GetError() );
                success = false;
            }
            */

            /*
            SDL_MinimizeWindow( gWindow );
            SDL_Delay( 2000 );
            SDL_RestoreWindow( gWindow );
            SDL_Delay( 2000 );
            SDL_MaximizeWindow( gWindow );
            SDL_Delay( 2000 );
            SDL_RestoreWindow( gWindow );
            */


            /*
            SDL_Delay( 2000 );
            SDL_SetWindowBordered( gWindow, SDL_TRUE );
            SDL_Delay( 2000 );
            SDL_SetWindowBordered( gWindow, SDL_FALSE );
            */


            //SDL_EnableScreenSaver();
            // Wait two seconds
            //SDL_Delay( 2000 );
            //SDL_DisableScreenSaver();

        }
        else {
            SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
    }
    return success;
}


bool loadSplashImage()
{
    gSplashImage = IMG_Load( splashImageFileName );
    if( gSplashImage == NULL ) {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Unable to load %s! SDL Error: %s\n", splashImageFileName, SDL_GetError() );
        return false;
    }
    //SDL_SetColorKey( gSplashImage, SDL_TRUE, SDL_MapRGB(gSplashImage->format, 0, 255, 0) );
    SDL_SetColorKey( gSplashImage, SDL_TRUE, SDL_MapRGB(gSplashImage->format, 0x00, 0x66, 0x00) );
    return true;
}


void cleanupAndExit()
{
    /*
    // free sound effects
    Mix_FreeChunk( gScratch );
    Mix_FreeChunk( gHigh );
    Mix_FreeChunk( gMedium );
    Mix_FreeChunk( gLow );
    gScratch = NULL;
    gHigh = NULL;
    gMedium = NULL;
    gLow = NULL;
    */

    // free music
    Mix_FreeMusic( gMusic );
    gMusic = NULL;

    // deallocate surface
    SDL_FreeSurface( gSplashImage );
    gSplashImage = NULL;

    // destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    // quit SDL subsystems
    SDL_Quit();
}




int main(/*int argc, char *argv[]*/)
{
    if( !init() ) {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "FATAL ERROR: Failed to initialize SDL! Returning...\n" );
        return 1;
    }

    DEBUG && getPlatformAndCPUInformation();
    DEBUG && getAllVideoDriversInfo();
    DEBUG && getAvailableDisplaysAndTheirModes();
    DEBUG && getWindowFlags();

    if( !loadSplashImage() ) {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to load splash image! Returning...\n" );
        return 1;
    } else {
        // apply image
        SDL_BlitSurface( gSplashImage, NULL, gScreenSurface, NULL );
        // update surface
        SDL_UpdateWindowSurface( gWindow );
    }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "SDL_mixer could not initialize! SDL_mixer Error: %s\nReturning...\n", Mix_GetError() );
        return 1;
    }
    // load music
    gMusic = Mix_LoadMUS( splashAudioFileName );
    if( gMusic == NULL ) {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to load music %s! SDL_mixer Error: %s\nReturning...\n", splashAudioFileName, Mix_GetError() );
        return 1;
    }
    // play music
    Mix_PlayMusic( gMusic, -1 );

/*
    // load sound effects
    gScratch = Mix_LoadWAV( scratchEffectFileName );
    if( gScratch == NULL ) {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to load sound effect %s! SDL_mixer Error: %s\nReturning...\n", scratchEffectFileName, Mix_GetError() );
        return 1;
    }
    gHigh = Mix_LoadWAV( highEffectFileName );
    if( gHigh == NULL ) {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to load sound effect %s! SDL_mixer Error: %s\nReturning...\n", highEffectFileName, Mix_GetError() );
        return 1;
    }
    gMedium = Mix_LoadWAV( mediumEffectFileName );
    if( gMedium == NULL ) {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to load sound effect %s! SDL_mixer Error: %s\nReturning...\n", mediumEffectFileName, Mix_GetError() );
        return 1;
    }
    gLow = Mix_LoadWAV( lowEffectFileName );
    if( gLow == NULL ) {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to load sound effect %s! SDL_mixer Error: %s\nReturning...\n", lowEffectFileName, Mix_GetError() );
        return 1;
    }
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////



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

    cleanupAndExit();

    return 0;
}
