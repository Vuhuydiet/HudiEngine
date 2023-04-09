#pragma once

#include "Hudi/Renderer/Shader.h"

#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Hudi {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string & vertexSource, const std::string& fragmentSource);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniform(const std::string& name, int value) override;
		virtual void SetUniform(const std::string& name, int* values, size_t count) override;
		virtual void SetUniform(const std::string& name, const glm::vec3& value) override;
		virtual void SetUniform(const std::string& name, const glm::vec4& value) override;
		virtual void SetUniform(const std::string& name, const glm::mat4& value) override;

		void UploadUniform(const std::string& name, int value);
		void UploadUniform(const std::string& name, int* values, size_t count);
		void UploadUniform(const std::string& name, float value);
		void UploadUniform(const std::string& name, const glm::vec2& value);
		void UploadUniform(const std::string& name, const glm::vec3& value);
		void UploadUniform(const std::string& name, const glm::vec4& value);
		void UploadUniform(const std::string& name, const glm::mat3& value);
		void UploadUniform(const std::string& name, const glm::mat4& value);

		const std::string& GetName() const override { return m_Name; }

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& src);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSrcs);

		uint32_t GetLocation(const std::string& type, const std::string& name);
	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};

}