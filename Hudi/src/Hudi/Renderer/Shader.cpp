#include "hdpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hudi {

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HD_CORE_ASSERT(true, "RendererAPI \"None\" is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return NewRef<OpenGLShader>(filepath);
		}

		HD_CORE_ASSERT(true, "Unknown renderer API!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HD_CORE_ASSERT(true, "RendererAPI \"None\" is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return NewRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		HD_CORE_ASSERT(true, "Unknown renderer API!");
		return nullptr;
	}

	///////////////////////////////////////////////////////////////////////////////////
	/////// Shader Library ////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	

	void ShaderLibrary::Add(const std::string& name, Ref<Shader> shader)
	{
		HD_CORE_ASSERT(Exists(name), "Shader '{0} already exists!", name);
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(Ref<Shader> shader)
	{
		const std::string& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		Ref<Shader> shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		Ref<Shader> shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		Ref<Shader> shader = Shader::Create(name, vertexSrc, fragmentSrc);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		HD_CORE_ASSERT(!Exists(name), "Shader '{0}' not found!", name);
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}