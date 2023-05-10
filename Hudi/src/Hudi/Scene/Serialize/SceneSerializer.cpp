#include "hdpch.h"
#include "SceneSerializer.h"

#include <yaml-cpp/yaml.h>

namespace YAML {

	template <>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template <>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template <>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template <>
	struct convert<Hudi::Vec3>
	{
		static Node encode(const Hudi::Vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, Hudi::Vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template <>
	struct convert<Hudi::Camera::OrthographicData>
	{
		static Node encode(const Hudi::Camera::OrthographicData& rhs)
		{
			Node node;
			node.push_back(rhs.left);
			node.push_back(rhs.right);
			node.push_back(rhs.bottom);
			node.push_back(rhs.top);
			node.push_back(rhs.zNear);
			node.push_back(rhs.zFar);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, Hudi::Camera::OrthographicData& rhs)
		{
			if (!node.IsSequence() || node.size() != 6)
				return false;
			rhs.left		= node[0].as<float>();
			rhs.right		= node[1].as<float>();
			rhs.bottom		= node[2].as<float>();
			rhs.top			= node[3].as<float>();
			rhs.zNear		= node[4].as<float>();
			rhs.zFar		= node[5].as<float>();
			return true;
		}
	};

	template <>
	struct convert<Hudi::Camera::PerspectiveData>
	{
		static Node encode(const Hudi::Camera::PerspectiveData& rhs)
		{
			Node node;
			node.push_back(rhs.FOV);
			node.push_back(rhs.aspectRatio);
			node.push_back(rhs.zNear);
			node.push_back(rhs.zFar);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, Hudi::Camera::PerspectiveData& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.FOV			= node[0].as<float>();
			rhs.aspectRatio = node[1].as<float>();
			rhs.zNear		= node[2].as<float>();
			rhs.zFar		= node[3].as<float>();
			return true;
		}
	};

}

namespace Hudi {

	YAML::Emitter& operator<< (YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<< (YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<< (YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<< (YAML::Emitter& out, const Vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<< (YAML::Emitter& out, const Camera::OrthographicData& data)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << data.left << data.right << data.bottom << data.top << data.zNear << data.zFar << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<< (YAML::Emitter& out, const Camera::PerspectiveData& data)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << data.FOV << data.aspectRatio << data.zNear << data.zFar << YAML::EndSeq;
		return out;
	}

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
			out << YAML::Key << "Image file path" << YAML::Value << sprite.filepath;
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

		out << YAML::EndMap; // GameObject
	}

	void SceneSerializer::Save(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;
		m_Scene->Each([&](GameObject object) {
			SerializeGameObject(out, object, m_Scene->GetGameObjectName(object));
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
		fout.close();
	}

	bool SceneSerializer::Open(const std::string& filepath)
	{
		YAML::Node data = YAML::LoadFile(filepath);
		if (!data["Scene"])
		{
			HD_CORE_ERROR("No scene found in '{0}'!", filepath);
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();

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
		}
		return true;
	}

}