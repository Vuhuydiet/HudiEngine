#include "hdpch.h"
#include "SceneSerializer.h"

#include "EDHudiTypes.h"

#include <yaml-cpp/yaml.h>

namespace Hudi {

	SceneSerializer::SceneSerializer(Ref<Scene> scene)
		: m_Scene(scene)
	{
		HD_CORE_ASSERT(!scene, "Cannot serialize a null scene!");
	}

	static void SerializeGameObject(YAML::Emitter& out, GameObject object, const std::string& name)
	{
		out << YAML::BeginMap; // GameObject
		out << YAML::Key << "Entity" << YAML::Value << object.GetEntityID();

		out << YAML::Key << "Name" << YAML::Value << name;

		if (object.HasComponent<Transform>())
		{
			const auto& transform = object.GetComponent<Transform>();
			out << YAML::Key << "Transform";
			out << YAML::BeginMap;
			out << YAML::Key << "Position" << YAML::Value << transform.position;
			out << YAML::Key << "Rotation" << YAML::Value << transform.rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.scale;
			out << YAML::EndMap;
		}

		if (object.HasComponent<SpriteRenderer>())
		{
			const auto& sprite = object.GetComponent<SpriteRenderer>();
			out << YAML::Key << "Sprite Renderer";
			out << YAML::BeginMap;
			out << YAML::Key << "Size" << YAML::Value << sprite.size;
			out << YAML::Key << "Color" << YAML::Value << sprite.color;
			out << YAML::Key << "Image file path" << YAML::Value << sprite.filepath.string();
			out << YAML::Key << "Order" << YAML::Value << sprite.order;
			out << YAML::EndMap;
		}

		if (object.HasComponent<Camera>())
		{
			const auto& camera = object.GetComponent<Camera>();
			out << YAML::Key << "Camera";
			out << YAML::BeginMap;
			out << YAML::Key << "Type" << YAML::Value << (int)camera.GetType();
			out << YAML::Key << "Orthographic Data" << YAML::Value << camera.GetOrthographicData();
			out << YAML::Key << "Perspective Data" << YAML::Value << camera.GetPerspectiveData();
			out << YAML::EndMap;
		}

		if (object.HasComponent<Rigidbody2D>())
		{
			const auto& rb2 = object.GetComponent<Rigidbody2D>();
			out << YAML::Key << "Rigidbody 2D";
			out << YAML::BeginMap;
			out << YAML::Key << "Type" << YAML::Value << (int)rb2.type;
			out << YAML::Key << "Fixed Rotation" << YAML::Value << rb2.fixedRotation;
			out << YAML::EndMap;
		}

		if (object.HasComponent<BoxCollider2D>())
		{
			const auto& box2 = object.GetComponent<BoxCollider2D>();
			out << YAML::Key << "Box Collider 2D";
			out << YAML::BeginMap;
			out << YAML::Key << "Offset" << YAML::Value << box2.offset;
			out << YAML::Key << "Size" << YAML::Value << box2.size;
			out << YAML::Key << "Density" << YAML::Value << box2.density;
			out << YAML::Key << "Friction" << YAML::Value << box2.friction;
			out << YAML::Key << "Restitution" << YAML::Value << box2.restitution;
			out << YAML::Key << "Restitution Threshold" << YAML::Value << box2.restitutionThreshold;
			out << YAML::EndMap;
		}

		out << YAML::EndMap; // GameObject
	}

	void SceneSerializer::Save(const std::filesystem::path& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Primary Camera" << YAML::Value << m_Scene->GetGameObjectName(m_Scene->GetPrimaryCamera());
		out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;
		m_Scene->Each([&](GameObject object) {
			SerializeGameObject(out, object, m_Scene->GetGameObjectName(object));
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath.string());
		fout << out.c_str();
		fout.close();
	}

	bool SceneSerializer::Open(const std::filesystem::path& filepath)
	{
		YAML::Node data = YAML::LoadFile(filepath.string());
		if (!data["Scene"])
		{
			HD_CORE_ERROR("No scene found in '{0}'!", filepath);
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();
		std::string primaryCameraName = data["Primary Camera"].as<std::string>();

		YAML::Node objects = data["Objects"];
		if (!objects)
			return false;
		
		for (auto object : objects)
		{
			uint32_t id = object["Entity"].as<uint32_t>();
			std::string name = object["Name"].as<std::string>();

			GameObject newobject = m_Scene->CreateEmptyObject(name);

			YAML::Node transform = object["Transform"];
			if (transform)
			{
				Transform& transformComponent = newobject.GetOrAddComponent<Transform>();
				transformComponent.position = transform["Position"].as<Vec3>();
				transformComponent.rotation= transform["Rotation"].as<Vec3>();
				transformComponent.scale = transform["Scale"].as<Vec3>();
			}

			YAML::Node spriteRenderer = object["Sprite Renderer"];
			if (spriteRenderer)
			{
				SpriteRenderer& spriteComponent = newobject.GetOrAddComponent<SpriteRenderer>();
				spriteComponent.size = spriteRenderer["Size"].as<glm::vec2>();
				spriteComponent.color = spriteRenderer["Color"].as<glm::vec4>();
				spriteComponent.SetFilePath(spriteRenderer["Image file path"].as<std::string>());
				spriteComponent.order = spriteRenderer["Order"].as<int>();
			}

			YAML::Node camera = object["Camera"];
			if (camera)
			{
				Camera& cameraComponent = newobject.GetOrAddComponent<Camera>();
				cameraComponent.Resize(m_Scene->GetViewportSize());
				cameraComponent.Switch((Camera::Type)camera["Type"].as<int>());
				cameraComponent.SetProjectionData(camera["Orthographic Data"].as<Camera::OrthographicData>());
				cameraComponent.SetProjectionData(camera["Perspective Data"].as<Camera::PerspectiveData>());
			}

			YAML::Node rb2 = object["Rigidbody 2D"];
			if (rb2)
			{
				Rigidbody2D& rb2Component = newobject.GetOrAddComponent<Rigidbody2D>();
				rb2Component.type = (Rigidbody2D::BodyType)rb2["Type"].as<int>();
				rb2Component.fixedRotation = rb2["Fixed Rotation"].as<bool>();
			}

			YAML::Node box2 = object["Box Collider 2D"];
			if (box2)
			{
				BoxCollider2D& box2Component = newobject.GetOrAddComponent<BoxCollider2D>();
				box2Component.offset = box2["Offset"].as<glm::vec2>();
				box2Component.size = box2["Size"].as<glm::vec2>();
				box2Component.density = box2["Density"].as<float>();
				box2Component.friction = box2["Friction"].as<float>();
				box2Component.restitution = box2["Restitution"].as<float>();
				box2Component.restitutionThreshold = box2["Restitution Threshold"].as<float>();
			}
		}
		m_Scene->SetPrimaryCamera(primaryCameraName);

		return true;
	}

}