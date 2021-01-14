//
//  Lutra.h
//  Lutra
//
//  Created by JasonCheng on 2021/1/5.
//

#ifndef Lutra_h
#define Lutra_h

#include "Engine/Engine.h"

#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Scene/Scene.h"
#include "Scene/SceneObject.h"

#include "Components/Transform.h"
#include "Components/Tag.h"
#include "Components/SceneObjectDelegate.h"
#include "Components/Camera.h"
#include "Components/MeshFilter.h"
#include "Components/SpriteRenderer.h"
#include "Components/Serializable.h"

#include "Systems/TransformSystem.h"
#include "Systems/SpriteSystem.h"
#include "Systems/MeshFilterSystem.h"

#include "Resources/Mesh.h"
#include "Resources/Shader.h"
#include "Resources/Pass.h"
#include "Resources/Texture.h"
#include "Resources/Material.h"
#include "Resources/Mesh.h"
#include "Resources/Stream.h"
#include "Resources/ResourceManager.h"
#include "Resources/ResourceManifest.h"

#include "Parsers/SceneParser.h"
#include "Parsers/SceneLoader.h"

#include <gtc/type_ptr.hpp>

#endif /* Lutra_h */
