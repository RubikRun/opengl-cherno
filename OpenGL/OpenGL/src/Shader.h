#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
	unsigned int rendererId;
	std::string filepath;
	std::unordered_map<std::string, int> uniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform1i(const std::string& name, int v);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void setUniform1iv(const std::string& name, int* v);
private:
	int getUniformLocation(const std::string& name);
	ShaderProgramSource parseShader(const std::string& filepath) const;
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) const;
	unsigned int compileShader(const std::string& sourceStr, unsigned int type) const;
};