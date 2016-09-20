#pragma once

#include <irrlicht.h>

using namespace irr;

class HexagonNode : public scene::ISceneNode
{
protected:
	core::aabbox3d<f32> Box;
	irr::core::vector3d<f32> center;
	video::S3DVertex Vertices[7];	// for this case ... let's include the center
	video::SMaterial Material;

public: 
	static const int hexagon_sides = 6;

	HexagonNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, const irr::core::vector3d<f32>& center, double size);
	
	virtual void OnRegisterSceneNode();

	virtual void HexagonNode::render();

	virtual const core::aabbox3d<f32>& getBoundingBox() const;
	virtual u32 getMaterialCount() const;
	virtual video::SMaterial& getMaterial(u32 i);

private:
	inline irr::core::vector3d<f32> compute_corner(int i, f32 size) const
	{
		auto angle_deg = 60.0 * i + 30.0;
		auto angle_rad = core::PI / 180.0 * angle_deg;
		return irr::core::vector3d<f32>(
			this->center.X + size * cos(angle_rad),
			this->center.Y + size * sin(angle_rad),
			0);
	}
};
