#include "hexagon.h"

HexagonNode::HexagonNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, const irr::core::vector3d<f32>& center, double size)
		: scene::ISceneNode(parent, mgr, id)
{
		Material.Wireframe = false;
		Material.Lighting = false;
		this->center = center;
		
		auto normal  = core::vector3df(0, 1, 0);
		auto fake_uv = core::vector2d<f32>(0, 0);

		auto center_position = core::vector3df(center.X, center.Y, center.Z);
		Vertices[0] = video::S3DVertex(center_position, normal, video::SColor(255, 0, 255, 255), fake_uv);

		for(int index = 0; index < hexagon_sides; index ++)
		{
			auto hexagon_vertex_position = compute_corner(index, size); 
			auto position = core::vector3df(hexagon_vertex_position.X, hexagon_vertex_position.Y, hexagon_vertex_position.Z);
			Vertices[1 + index] = video::S3DVertex(position, normal,  video::SColor(255, 0, 255, 255), fake_uv);
		}

		Box.reset(center.X, center.Y, center.Z);
		for (s32 i = 0; i < hexagon_sides + 1; ++i)
			Box.addInternalPoint(Vertices[i].Pos);
}

void HexagonNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void HexagonNode::render()
{
	u16 indices[] = {	0,2,1,
						0,3,2,
						0,4,3,
						0,5,4,
						0,6,5,
						0,1,6
					};
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->drawVertexPrimitiveList(&Vertices[0], 
									hexagon_sides + 1, 
									&indices[0], 
									hexagon_sides + 1,
									video::EVT_STANDARD, 
									scene::EPT_TRIANGLES, 
									video::EIT_16BIT);
}

const core::aabbox3d<f32>& HexagonNode::getBoundingBox() const
{
	return Box;
}

u32 HexagonNode::getMaterialCount() const
{
	return 1;
}

video::SMaterial& HexagonNode::getMaterial(u32 i)
{
	return Material;
}
