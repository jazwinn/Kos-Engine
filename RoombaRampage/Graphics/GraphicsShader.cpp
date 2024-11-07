#include "../Config/pch.h"
#include "../Graphics/GraphicsPipe.h"

namespace graphicpipe
{

	GLuint GraphicsPipe::m_funcCompileShader(GLuint type, const std::string& shader)
	{
		GLuint lvID = glCreateShader(type);
		const char* source = shader.c_str();
		glShaderSource(lvID, 1, &source, nullptr);
		glCompileShader(lvID);

		int result;
		glGetShaderiv(lvID, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			int length;
			glGetShaderiv(lvID, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)malloc(length * sizeof(char));
			glGetShaderInfoLog(lvID, length, &length, message);
			std::cout << "Failed to Compile Shader" << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(lvID);
			free(message);
			//TOCHECK
			//std::exit;
			return 0;
		}

		return lvID;
	}

	unsigned int GraphicsPipe::m_funcSetupShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		GLuint lvProgram = glCreateProgram();
		GLuint lvVertexShaderID = m_funcCompileShader(GL_VERTEX_SHADER, vertexShader);
		GLuint lvFragmentShaderID = m_funcCompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(lvProgram, lvVertexShaderID);
		glAttachShader(lvProgram, lvFragmentShaderID);
		glLinkProgram(lvProgram);
		glValidateProgram(lvProgram);

		glDeleteShader(lvVertexShaderID);
		glDeleteShader(lvFragmentShaderID);

		GLint success;
		glGetProgramiv(lvProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[512];
			glGetProgramInfoLog(lvProgram, 512, NULL, infoLog);
			std::cout << "Error linking shader program:\n" << infoLog << std::endl;
		}
		else
		{
			LOGGING_INFO("Shader compiled successfully");
		}

		return lvProgram;
	}


	void GraphicsPipe::m_funcDeleteShader()
	{
		glDeleteProgram(m_genericShaderProgram);
	}
}