#include "shader.hpp"

class Square{

	GLuint VAO, VBO, EBO;
	Shader *shader;
	
public:
	
	Square(float x, float y){
		
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
    		std::cout << "Failed to initialize GLEW" << std::endl;
		}
		
		shader = new Shader("shaders/square.verx", "shaders/square.frag");
		
		GLfloat vertices [] = 
		{
			0.0f, 0.0f,
			x, 0.0f,
			0.0f, y,
			x, y
		};
		
		GLint indeces [] = { 0, 1, 2, 1, 2, 3 };
		
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		
		glBindVertexArray(VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		
		glBindBuffer(EBO, GL_ELEMENT_ARRAY_BUFFER);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_DYNAMIC_DRAW);
		
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);	
	}
	
	void drawing(){
		(*shader).Use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};
