#pragma once

// Enumerated type used for indexing and referencing events
enum EventName {
    EVENT_EXAMPLE, // Example event type for demonstration purposes
	RENDERER_ADD_TO_RENDERABLES,
    RENDERER_ADD_TO_UIRENDERABLES, // Tell renderer to add renderable to list
    RENDERER_POPULATE_BUFFERS, // Tell renderer to repopulate buffers for a renderable
    RENDERER_SET_CAMERA, // Set Camera Position
    RENDERER_REPOPULATE_BUFFERS,
    UI_CLICK, // Clicked on screen
	PLAYER_JUMP,   // Used to trigger a player movement event
	PLAYER_LEFT,
	PLAYER_RIGHT,
	WEAPON_PREV,
	WEAPON_NEXT,
	WEAPON_FIRE,
	AIM_LEFT,
	AIM_RIGHT,
    PLAY_SONG,
    PLAY_SOUND,
	PLAYER_FIRE,
    BULLET_SPAWN,
    GAME_START,
};
