// heavily modified, based on modern openGL tutorial:
// https://www.youtube.com/watch?v=aA112viAx7c&list=PLRtjMdoYXLf6zUMDJVRZYV-6g6n62vet8&index=14
// Also based on the learnopengl tutorials.

#pragma once

#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class ShaderProgram
{
public:
	// The program ID
	GLuint ProgramID;

	// Constructor. Generates the shader.
	ShaderProgram(const GLchar *pvertexPath, const GLchar *pfragmentPath)
	{
		// 1. Retrieve vertex and fragment sourcecode.
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile; // vertex
		std::ifstream fShaderFile; // fragment (sometimes called "pixel")

		// Ensure ifstream can throw exceptions if necessary:
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			// Open the files.
			vShaderFile.open(pvertexPath);
			fShaderFile.open(pfragmentPath);

			// Read files' buffer contents into streams.
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			// Close the file handlers.
			vShaderFile.close();
			fShaderFile.close();

			// Convert the stream into a string.
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure exc)
		{
			std::cerr << "ERROR::SHADER::FILE_NOT_FOUND_SUCCESFULLY_READ" << std::endl; // Tutorial had this as cout. I felt cerr is more appropriate.
		}

		const GLchar *pvShaderCode = vertexCode.c_str();
		const GLchar *pfShaderCode = fragmentCode.c_str();

		// 2. Compile shaders.
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];

		// Vertex Shader.
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &pvShaderCode, NULL);
		glCompileShader(vertex);

		// Print compile errors if there are any.
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
					  << infoLog << std::endl; // Tutorial had this as cout. I felt cerr is more appropriate.
		}

		// Fragment Shader.
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &pfShaderCode, NULL);
		glCompileShader(fragment);

		// Again, print compile errors.
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
					  << infoLog << std::endl; // Tutorial had this as cout. I felt cerr is more appropriate.
		}

		// The Shader Program.
		this->ProgramID = glCreateProgram();
		glAttachShader(this->ProgramID, vertex);
		glAttachShader(this->ProgramID, fragment);
		glLinkProgram(this->ProgramID);

		// Print linking errors if any occur.
		glGetProgramiv(this->ProgramID, GL_LINK_STATUS, &success); // Gets the link status from the progam-- is it succesful?
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
					  << infoLog << std::endl; // Tutorial had this as cout. I felt cerr is more appropriate.
		}

		// Delete the shaders now that they're linked.
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	} // End of constructor.
	// Use the current shader.
	void Use()
	{
		glUseProgram(this->ProgramID);
	}
	void unUse()
	{
		glUseProgram(0);
	}
	// Uniform setter functions.
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ProgramID, name.c_str()), value);
	}
};