#pragma once

// Enumerated type used for indexing and referencing events
enum EventName {
    EVENT_EXAMPLE, // Example event type for demonstration purposes
    RENDERER_ADD_TO_UIRENDERABLES, // Tell renderer to add renderable to list
    RENDERER_POPULATE_BUFFERS, // Tell renderer to repopulate buffers for a renderable
    RENDERER_INIT_FONT, // Initialize a font texture
    RENDERER_CREATE_UI_TEXTURE, // Create Texture for UIComponent
};