#include "Hudi/Source/Vec3.h"
#include "Hudi/Scene/Components/CameraComponent.h"
#include <glm/glm.hpp>

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
			rhs.left = node[0].as<float>();
			rhs.right = node[1].as<float>();
			rhs.bottom = node[2].as<float>();
			rhs.top = node[3].as<float>();
			rhs.zNear = node[4].as<float>();
			rhs.zFar = node[5].as<float>();
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
			rhs.FOV = node[0].as<float>();
			rhs.aspectRatio = node[1].as<float>();
			rhs.zNear = node[2].as<float>();
			rhs.zFar = node[3].as<float>();
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

}