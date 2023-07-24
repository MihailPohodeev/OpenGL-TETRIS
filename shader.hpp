#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

using namespace std;

class Shader_{
	
public:
	GLuint Program;
	
	void Use() {glUseProgram(this->Program);}
	
	Shader_(const GLchar* vertexPath, const GLchar* fragmentPath){
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;
		
		vShaderFile.exceptions(ifstream::badbit);
		fShaderFile.exceptions(ifstream::badbit);
		
		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			stringstream vShaderStream, fShaderStream;
			
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			
			vShaderFile.close();
			fShaderFile.close();
			
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch(ifstream::failure e)
		{
			cout<<"ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ"<<'\n';
		}
		
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();
		
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];
		
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"<<infoLog<<'\n';
		}
		
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			cout<<"ERROR::SHADER::FRAGMENTAL::COMPILATION_FAILED\n"<<infoLog<<'\n';
		}
		
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);
		
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success){
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			cout<<"ERROR::SHADER::PROGRAM::LINKING_FAILED\n"<<infoLog<<'\n';
		}
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}	
};


































