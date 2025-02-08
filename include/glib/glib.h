#pragma once
// This is the only header file you need to include for most features.

#include <string>
#include <functional>

#include "DLLDefs.h"

#include "Instance.h"
#include "glibError.h"
#include "window/Window.h"
#include "utils/MouseButton.h"
#include "utils/KeyCode.h"
#include "utils/Face.h"
#include "tween/Easing.h"
#include "utils/Color.h"
#include "utils/Utils.h"
#include "sound/SoundManager.h"
#include "sound/Sound.h"
#include "sound/AudioDataSource.h"
#include "sound/effect/ReverbEffect.h"
#include "sound/effect/SoundEffect.h"
#include "scene/SceneManager.h"
#include "scene/Scene.h"
#include "physics/PhysicsManager.h"
#include "physics/PhysicsObject.h"
#include "physics/component/BoxCollider.h"
#include "physics/component/PhysicsComponent.h"
#include "math/Axis.h"
#include "math/Mat4.h"
#include "math/MathFunctions.h"
#include "math/Rect.h"
#include "math/Vec2.h"
#include "math/Vec2i.h"
#include "graphics/Texture.h"
#include "graphics/Text.h"
#include "graphics/Sprite.h"
#include "graphics/Shader.h"
#include "graphics/Font.h"
#include "graphics/Drawable.h"
#include "graphics/video/VideoPlayer.h"
#include "graphics/pipeline/CameraRenderer.h"
#include "graphics/pipeline/PipelineElement.h"
#include "graphics/pipeline/RenderPipeline.h"
#include "graphics/pipeline/WindowRenderer.h"
#include "framebuffer/Framebuffer.h"
#include "event/Event.h"
#include "event/EventManager.h"
#include "event/EventSubscriber.h"
#include "animation/Animatable.h"
#include "animation/Animation.h"
#include "animation/AnimationManager.h"
#include "animation/loader/SparrowAtlasLoader.h"
#include "backend/Backend.h"
#include "graphics/postprocessing/PostProcessing.h"
#include "glib/utils/glibGlobals.h"
#include "glib/graphics/ui/UI.h"

/**
* glib (game library) is a game library aiming to make it easier to create games with C++.
*/
namespace glib
{
	/**
	* Initializes glib by creating a new Instance.
	* 
	* @param backend - The backend for the graphics and sound apis (Use Backend::GetBackend("backend name"))
	* 
	* @returns A new glib instance
	*/
	GLIB_API Instance* Init(Backend* backend);

	/**
	* the same as: glib::Init(Backend::GetBackend("opengl"));
	*/
	GLIB_API void SimpleInit();

	/**
	* Terminates glib by deleting the Instance. This also frees any memory allocated by glib.
	*/
	GLIB_API void Terminate();
}