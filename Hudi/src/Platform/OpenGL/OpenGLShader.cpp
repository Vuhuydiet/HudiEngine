#include "hdpch.h"
#include "OpenGLShader.h"

#include "Hudi/Core/Core.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Hudi {

	static GLenum StringToGLShaderType(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		HD_CORE_ASSERT(true, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string shaderSrc = ReadFile(filepath);
		std::unordered_map<GLenum, std::string> shaderSrcs = PreProcess(shaderSrc);
		Compile(shaderSrcs);

		// Get shader name form filepath (assets/shaders/texture.glsl)
		size_t lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		size_t lastDot = filepath.rfind('.');
		size_t count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> shaderSrcs;
		shaderSrcs[GL_VERTEX_SHADER] = vertexSource;
		shaderSrcs[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(shaderSrcs);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (!in)
		{
			HD_CORE_ERROR("Could not open file '{0}'!", filepath);
		}

		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& src)
	{
		std::unordered_map<GLenum, std::string> shaderSrcs;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = src.find(typeToken, 0);
		while(pos != std::string::npos)
		{
			size_t eol = src.find_first_of("\r\n", pos);
			HD_CORE_ASSERT(eol == std::string::npos, "Syntax error!");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = src.substr(begin, eol - begin);
			HD_CORE_ASSERT(!StringToGLShaderType(type), "invalid shader type!");

			size_t nextLinePos = src.find_first_not_of("\r\n", eol);
			pos = src.find(typeToken, nextLinePos);
			shaderSrcs[StringToGLShaderType(type)] = src.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? src.size() - 1 : nextLinePos));
		}
		return shaderSrcs;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSrcs)
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaders;
		for (const auto& [type, src] : shaderSrcs)
		{
			GLuint shader = glCreateShader(type);
			const GLchar* source = src.c_str();
			glShaderSource(shader, 1, &source, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				for (const auto& shad : shaders)
				{
					glDeleteShader(shad);
				}
				glDeleteProgram(program);

				HD_CORE_ERROR("{0}", infoLog.data());
				HD_CORE_ASSERT(true, "Shader compilation failed!");
				return;
			}
			glAttachShader(program, shader);
			shaders.push_back(shader);
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			for (const auto& shad : shaders)
			{
				glDeleteShader(shad);
			}

			HD_CORE_ERROR("{0}", infoLog.data());
			HD_CORE_ASSERT(true, "Shader program linking error!");
			return;
		}
			
		m_RendererID = program;

		for (const auto& shad : shaders)
		{
			glDetachShader(program, shad);
		}
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetUniform(const std::string& name, int value)
	{
		UploadUniform(name, value);
	}


	void OpenGLShader::SetUniform(const std::string& name, const glm::vec3& value)
	{
		UploadUniform(name, value);
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::vec4& value)
	{
		UploadUniform(name, value);
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::mat4& value)
	{
		UploadUniform(name, value);
	}

	uint32_t OpenGLShader::GetLocation(const std::string& type, const std::string& name)
	{
		glUseProgram(m_RendererID);
		int location = glGetUniformLocation(m_RendererID, name.c_str());

		if (location < 0)
		{
			HD_CORE_ERROR("No such '{0}' uniform named '{1}', misspelled or was not be used?", type, name);
		}
		return location;
	}

	void OpenGLShader::UploadUniform(const std::string& name, int value)
	{
		int location = GetLocation("int", name);

		glUseProgram(m_RendererID);
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniform(const std::string& name, float value)
	{
		int location = GetLocation("float", name);

		glUseProgram(m_RendererID);
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::vec2& value)
	{
		int location = GetLocation("vec2", name);

		glUseProgram(m_RendererID);
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::vec3& value)
	{
		int location = GetLocation("vec3", name);

		glUseProgram(m_RendererID);
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::vec4& value)
	{
		int location = GetLocation("vec4", name);

		glUseProgram(m_RendererID);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::mat3& value)
	{
		int location = GetLocation("mat3", name);

		glUseProgram(m_RendererID);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::mat4& value)
	{
		int location = GetLocation("mat4", name);

		glUseProgram(m_RendererID);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

}