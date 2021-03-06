//
//  GameLogicSystem.cpp
//  EngineTesting
//
//  Created by Hakob on 11/13/14.
//  Copyright (c) 2014 Haksist. All rights reserved.
//

#include <algorithm>

#include <iostream>

#include "../CoreEngine/Application.h"
#include "RenderSystem.h"
#include "../Debug/Logger.h"
#include "../CoreEngine/Transform.h"
#include "../Components/Renderer.h"
#include "../Components/Camera.h"
#include "../CoreEngine/Utils.h"

static bool sortRenderers(const Renderer* lhs, const Renderer* rhs)
{
	if (lhs->getSortingLayer() < rhs->getSortingLayer()){return true;}
	if (lhs->getSortingLayer() == rhs->getSortingLayer() && lhs->getLayerOrder() < rhs->getLayerOrder()){ return true; };

	return false;
}

RenderSystem::RenderSystem()
{	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_CULL_FACE);
#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_MAC)
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif

	//glEnable(GL_DEPTH_TEST);
	//glShadeModel(GL_SMOOTH);
	glFrontFace(GL_CCW);
}

RenderSystem::~RenderSystem()
{
	std::cout << "RenderSystem:: ~~~deleted~~~" << std::endl;
}

void RenderSystem::OnCreate()
{
	std::cout << "RenderSystem:: Init()" << std::endl;
}

void RenderSystem::Update()
{
	//std::cout << "RenderSystem:: Update() " << std::endl;

	//#TODO get enabled cameras from ScreenSystem and apply multiple cameras
	if (Camera::main == NULL)
		return;
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	static Matrix4 ModelMatrix, ViewMatrix, ModelViewMatrix, ModelViewProjectionMatrix;

	Matrix4& ProjectionMatrix = Camera::main->getProjectionMatrix();
	Camera::main->getViewMatrix(ViewMatrix);

 	for (size_t i = 0, len = Material::allMaterials_.size(); i < len; i++)
 	{
 		Material* mat = Material::allMaterials_[i];
 
		mat->bind();
		{
 			for (size_t r = 0, rlen = mat->sharingInfo_.size(); r < rlen; r++)
 			{
				Renderer* rend = mat->sharingInfo_[r].rend;
				if (rend->isEnabled())
				{
					Transform* transform = rend->getTransform();

					transform->getMatrix(ModelMatrix, true);
					Matrix4::MultiplyMatrices(ViewMatrix, ModelMatrix, ModelViewMatrix);
					Matrix4::MultiplyMatrices(ProjectionMatrix, ModelViewMatrix, ModelViewProjectionMatrix);

					mat->getShader()->setUniformMatrix4fv("ModelViewProjectionMatrix", 1, false, &ModelViewProjectionMatrix[0]);
					rend->Render(mat->sharingInfo_[r].materialIndex);
				}
 			}
		}
 		mat->unbind();
 	}

// 	Renderer* r = NULL;
// 
// 	for (size_t i = 0, len = components.size(); i < len; i++)
// 	{
// 		if (r != NULL)
// 		{
// 			if (r->getSortingLayer() != components[i]->getSortingLayer() ||
// 				r->getLayerOrder() != components[i]->getLayerOrder())
// 			{
// 				glClear(GL_DEPTH_BUFFER_BIT);
// 			}
// 		}
// 
// 		glPushMatrix();
// 		components[i]->Render();
// 		glPopMatrix();
// 		r = components[i];
// 	}

	glDisable(GL_BLEND);
}

void RenderSystem::OnStartFrame()
{
	System::OnStartFrame();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderSystem::OnEndFrame()
{
	System::OnEndFrame();

	for (std::pair< smart_pointer<Material>, MaterialShareInfo> pair : componentsToShare)
		pair.first->share(MaterialShareInfo(pair.second.rend, pair.second.materialIndex));

	componentsToShare.clear();
}

void RenderSystem::addComponent(Component &c)
{
	if (isSystemComponent(c))
	{
		static_cast<Renderer*>(&c)->renderSystem_ = this;
		addToBuffer(&c);
	}
}

void RenderSystem::removeComponent(Component &c)
{
	if (isSystemComponent(c))
	{
		componentsList_.remove(static_cast<Renderer*>(&c)->renderSystemNode_);
		removeFromBuffer(&c);
	}
}

bool RenderSystem::isSystemComponent(Component &c)
{
	return dynamic_cast<Renderer*>(&c) != NULL;
}

void RenderSystem::OnBufferChange(std::vector<Component*>& components)
{
	for (Component* component : components)
	{
		component->Init();
		componentsList_.addToBack(static_cast<Renderer*>(component)->renderSystemNode_);
	}

	sortComponents();
}

std::vector<Component*> RenderSystem::getComponents()
{
	vector<Component*> components;

	DoubleLinkedList<Renderer>::iterator iter = componentsList_.getIterator();
	while (iter.hasNext())
		components.push_back(iter.next()->data);

	return components;
}

void RenderSystem::sortComponents()
{
	
}

void RenderSystem::addMaterialForRenderer(smart_pointer<Material> mat, Renderer* rend, int materialIndex)
{
	componentsToShare.push_back(std::pair< smart_pointer<Material>, MaterialShareInfo>(mat, MaterialShareInfo(rend, materialIndex)));
}

void RenderSystem::removeMaterialForRenderer(smart_pointer<Material> mat, Renderer* rend, int materialIndex)
{
	std::vector<std::pair< smart_pointer<Material>, MaterialShareInfo>>::iterator iter;
	for (iter = componentsToShare.begin(); iter != componentsToShare.end(); iter++)
	{
		if (iter->first == mat && iter->second.rend == rend && iter->second.materialIndex == materialIndex)
		{
			componentsToShare.erase(iter);
			return;
		}
	}

	mat->unshare(MaterialShareInfo(rend, materialIndex));
}

int RenderSystem::getMaterialSharesCount(smart_pointer<Material> mat)
{
	return mat->getSharesCount();
}