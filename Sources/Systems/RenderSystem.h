
#ifndef EngineTesting_RenderSystem_h
#define EngineTesting_RenderSystem_h

#include <vector>
#include <list>
#include <utility>

#include "../CoreEngine/System.h"
#include "../Extras/smart_pointer.h"
#include "../Extras/DoubleLinkedList.h"

class Renderer;
class Material;

class MaterialShareInfo
{
public:
	Renderer* rend = NULL;
	int materialIndex = 0;
	MaterialShareInfo(Renderer* r, int matIndex) :rend(r), materialIndex(matIndex){};
};

class RenderSystem :public System
{
private:
	DoubleLinkedList<Renderer> componentsList_;
	std::vector<std::pair< smart_pointer<Material>, MaterialShareInfo>> componentsToShare;
public:
	~RenderSystem();
	RenderSystem();

	virtual void OnCreate();
	virtual void Update();
	virtual void OnStartFrame();
	virtual void OnEndFrame();
	virtual void addComponent(Component &c);
	virtual void removeComponent(Component &c);
	virtual bool isSystemComponent(Component &c);
	virtual std::vector<Component*> getComponents();

	void sortComponents();
	void addMaterialForRenderer(smart_pointer<Material> mat, Renderer* rend, int matIndex = 0);
	void removeMaterialForRenderer(smart_pointer<Material> mat, Renderer* rend, int matIndex = 0);
	int getMaterialSharesCount(smart_pointer<Material> mat);
protected:
	virtual void OnBufferChange(std::vector<Component*>& components);
};

#endif
