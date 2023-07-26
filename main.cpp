#include <GL/glew.h>
#include <GL/glfw3.h>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <time.h>
#include "shader.hpp"
#include <typeinfo>

using namespace std;
using namespace sf;

// единичная матрица
glm::mat4 unit_matrix = glm::mat4(1.0f, .0f, .0f, .0f, .0f, 1.0f, .0f, .0f, .0f, .0f, 1.0f, .0f, .0f, .0f, .0f, 1.0f);

// размер окна
int WIDTH = 200, HEIGHT = 400;

// функция для получения случайного числа из промежутка
int GetRandomNumber(int min, int max)
{
	
  srand(time(NULL));

  int num = min + rand() % (max - min + 1);

  return num;
}

// класс элемента, представляющего из себя прямоугольник с заданными длиной и шириной
class element{
		
		// массив аттрибутов вершин, вершинный буффер, индексный буффер соответственно
		GLuint VAO, VBO, EBO;
		// шейдер
		Shader_ *shader;
		
		// адреса юниформ переменных вершинного и фрагментарного шейдеров
		GLint transformLoc, modelLoc, viewLoc, projectionLoc, colorLoc;
		
	public:
		
		// позиция вершины
		glm::vec2 position;
		
		// матрицы для трансформации элемента
		glm::mat4 transform, model, view, projection;
		// цвет
		glm::vec4 color;
		
		// конструктор
		element(float width, float height){
			
			position = glm::vec2(0.f, 0.f);
			
			shader = new Shader_("shaders/square.verx", "shaders/square.frag");
			// задание матрицам стандартных начальных значений
			transform = model = view = projection = unit_matrix;
			// изменение размера элемента под заданные значения ширины и длины
			model = glm::scale(model, glm::vec3(width / WIDTH, height / HEIGHT, 1.f));
			
			transformLoc = glGetUniformLocation((*shader).Program, "transform");
			modelLoc = glGetUniformLocation((*shader).Program, "model");
			viewLoc = glGetUniformLocation((*shader).Program, "view");
			projectionLoc = glGetUniformLocation((*shader).Program, "projection");
			colorLoc = glGetUniformLocation((*shader).Program, "input_color");
			
			//стандартный цвет
			color = glm::vec4(1.f, 1.f, 1.f, 1.f);
			
			// вершины
			GLfloat vertices [] =
			{
				0.0f, 0.0f,
				2.0f, 0.0f,
				0.0f, 2.0f,
				2.0f, 2.0f
			};
			
			// индексы для отрисовки вершин
			GLuint indeces [] = {0, 1, 2, 1, 2, 3};
			
			// генерация буфферов и массива атрибутов
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);
			
			// передача данных буферам
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_DYNAMIC_DRAW);
			
			// создание аттрибута позиций вершин
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			
			// отвязка буффера
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			
	}
	
	element(const element &el)
    {
    		position = el.position;
			
			shader = new Shader_("shaders/square.verx", "shaders/square.frag");
			// задание матрицам стандартных начальных значений
			transform = model = view = projection = unit_matrix;
			// изменение размера элемента под заданные значения ширины и длины
			model = el.model;
			
			transformLoc = glGetUniformLocation((*shader).Program, "transform");
			modelLoc = glGetUniformLocation((*shader).Program, "model");
			viewLoc = glGetUniformLocation((*shader).Program, "view");
			projectionLoc = glGetUniformLocation((*shader).Program, "projection");
			colorLoc = glGetUniformLocation((*shader).Program, "input_color");
			
			//стандартный цвет
			color = el.color;
			
			// вершины
			GLfloat vertices [] =
			{
				0.0f, 0.0f,
				2.0f, 0.0f,
				0.0f, 2.0f,
				2.0f, 2.0f
			};
			
			// индексы для отрисовки вершин
			GLuint indeces [] = {0, 1, 2, 1, 2, 3};
        // генерация буфферов и массива атрибутов
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);
			
			// передача данных буферам
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_DYNAMIC_DRAW);
			
			// создание аттрибута позиций вершин
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			
			// отвязка буффера
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			
			this->setPosition(position);
    }
		
		// вывод данных на экран
		void draw(){
			
			shader->Use();
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
			glUniform4f(colorLoc, color.x, color.y, color.z, color.w);
			
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		
		// функция сдвига
		void move(glm::vec2 move_vec){
			position += move_vec;
			transform = glm::translate(transform, glm::vec3(move_vec.x * 2/ WIDTH, move_vec.y * 2 / HEIGHT, 0.f));
		}
		
		// функция установки позиции
		void setPosition(glm::vec2 pos){
			position = pos;
			transform = glm::translate(unit_matrix, glm::vec3(pos.x * 2 / WIDTH, pos.y * 2 / HEIGHT, .0f));
		}
		
		// получение текущей позиции на экране
		glm::vec2 getPosition() {return position;}
		
};

class figure{
protected:
	
	// массив элементов
	glm::vec2 position;
	
public:
	
	element *elem_array[4];
	
	// конструктов
	figure(){
		for (int i = 0; i < 4; i++){
			elem_array[i] = new element(18, 18);
		}
		position = glm::vec2(0.f, 0.f);
	}
	
	// отрисовка
	void draw(){
		for (int i = 0; i < 4; i++){
			elem_array[i]->draw();
		}
	}
	
	// сдвиг
	void move(glm::vec2 moving){
		position += moving;
		for (int i = 0; i < 4; i++){
			elem_array[i]->move(moving);
		}		
	}
	
	// установка позиции
	void setPosition(glm::vec2 pos){
		position = pos;
		glm::vec2 elem1_pos = elem_array[0]->getPosition();
		for (int i = 0; i < 4; i++){
			elem_array[i]->setPosition(elem_array[i]->getPosition() - elem1_pos + pos);
		}
	}
	
	glm::vec2 getPosition() {return position;}
	
	virtual void rotate() {;}
	virtual void rotate_back() {;}

};

// фигура в виде "О"
class O_figure : public figure{

public:
	O_figure() : figure(){
		
		for(int i = 0; i < 4; i++){
			elem_array[i]->color = glm::vec4(.16f, .4f, .88f, 1.f);
		}
		
		elem_array[0]->setPosition(glm::vec2(0.f, 0.f));
		elem_array[1]->setPosition(glm::vec2(20.f, 0.f));
		elem_array[2]->setPosition(glm::vec2(0.f, 20.f));
		elem_array[3]->setPosition(glm::vec2(20.f, 20.f));
	}
};

class I_figure : public figure{
	
	glm::vec2 rot_positions[2][4];
	int rot_index;
	
public:
	I_figure() : figure(){
		
		for(int i = 0; i < 4; i++){
			elem_array[i]->color = glm::vec4(.88f, .81f, .16f, 1.f);
		}
		
		rot_positions[0][0] = glm::vec2(0.f, 0.f);
		rot_positions[0][1] = glm::vec2(0.f, 20.f);
		rot_positions[0][2] = glm::vec2(0.f, 40.f);
		rot_positions[0][3] = glm::vec2(0.f, 60.f);
		
		rot_positions[1][0] = glm::vec2(0.f, 0.f);
		rot_positions[1][1] = glm::vec2(20.f, 0.f);
		rot_positions[1][2] = glm::vec2(40.f, 0.f);
		rot_positions[1][3] = glm::vec2(60.f, 0.f);
		
		elem_array[0]->setPosition(rot_positions[0][0]);
		elem_array[1]->setPosition(rot_positions[0][1]);
		elem_array[2]->setPosition(rot_positions[0][2]);
		elem_array[3]->setPosition(rot_positions[0][3]);
		
		rot_index = 1;
	}
	
	void rotate(){
		
		elem_array[0]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][0]);
		elem_array[1]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][1]);
		elem_array[2]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][2]);
		elem_array[3]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][3]);
		
		rot_index++;
		if (rot_index >= 2) rot_index = 0;
	}
	
	void rotate_back(){
		rot_index -= 2;
		if (rot_index < 0){
			rot_index = 2 + rot_index;
		}
		
		elem_array[0]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][0]);
		elem_array[1]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][1]);
		elem_array[2]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][2]);
		elem_array[3]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][3]);
		
		rot_index++;
		if (rot_index >= 2) rot_index = 0;
	}
	
};

// фигура в виде "S"
class S_figure : public figure{
	
	glm::vec2 rot_positions[2][4];
	int rot_index;
	
public:
	S_figure() : figure(){
		
		for(int i = 0; i < 4; i++){
			elem_array[i]->color = glm::vec4(.08f, .84f, .84f, 1.f);
		}
		
		rot_positions[0][0] = glm::vec2(0.f, 0.f);
		rot_positions[0][1] = glm::vec2(20.f, 0.f);
		rot_positions[0][2] = glm::vec2(20.f, 20.f);
		rot_positions[0][3] = glm::vec2(40.f, 20.f);
		
		rot_positions[1][0] = glm::vec2(0.f, 0.f);
		rot_positions[1][1] = glm::vec2(0.f, 20.f);
		rot_positions[1][2] = glm::vec2(-20.f, 20.f);
		rot_positions[1][3] = glm::vec2(-20.f, 40.f);
		
		elem_array[0]->setPosition(rot_positions[0][0]);
		elem_array[1]->setPosition(rot_positions[0][1]);
		elem_array[2]->setPosition(rot_positions[0][2]);
		elem_array[3]->setPosition(rot_positions[0][3]);
		
		rot_index = 1;
	}
	
	void rotate(){
		
		elem_array[0]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][0]);
		elem_array[1]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][1]);
		elem_array[2]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][2]);
		elem_array[3]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][3]);
		
		rot_index++;
		if (rot_index >= 2) rot_index = 0;
	}
	void rotate_back(){
		rot_index -= 2;
		if (rot_index < 0){
			rot_index = 2 + rot_index;
		}
		
		elem_array[0]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][0]);
		elem_array[1]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][1]);
		elem_array[2]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][2]);
		elem_array[3]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][3]);
		
		rot_index++;
		if (rot_index >= 2) rot_index = 0;
	}
};

// фигура в виде "Z"
class Z_figure : public figure{
	
	glm::vec2 rot_positions[2][4];
	int rot_index;
	
public:
	Z_figure() : figure(){
		
		for(int i = 0; i < 4; i++){
			elem_array[i]->color = glm::vec4(.83f, .08f, .84f, 1.f);
		}
		
		rot_positions[0][0] = glm::vec2(0.f, 0.f);
		rot_positions[0][1] = glm::vec2(20.f, 0.f);
		rot_positions[0][2] = glm::vec2(0.f, 20.f);
		rot_positions[0][3] = glm::vec2(-20.f, 20.f);
		
		rot_positions[1][0] = glm::vec2(0.f, 0.f);
		rot_positions[1][1] = glm::vec2(0.f, 20.f);
		rot_positions[1][2] = glm::vec2(20.f, 20.f);
		rot_positions[1][3] = glm::vec2(20.f, 40.f);
		
		elem_array[0]->setPosition(rot_positions[0][0]);
		elem_array[1]->setPosition(rot_positions[0][1]);
		elem_array[2]->setPosition(rot_positions[0][2]);
		elem_array[3]->setPosition(rot_positions[0][3]);
		
		rot_index = 1;
	}
	
	void rotate(){
		
		elem_array[0]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][0]);
		elem_array[1]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][1]);
		elem_array[2]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][2]);
		elem_array[3]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][3]);
		
		rot_index++;
		if (rot_index >= 2) rot_index = 0;
	}
	
	void rotate_back(){
		rot_index -= 2;
		if (rot_index < 0){
			rot_index = 2 + rot_index;
		}
		
		elem_array[0]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][0]);
		elem_array[1]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][1]);
		elem_array[2]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][2]);
		elem_array[3]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][3]);
		
		rot_index++;
		if (rot_index >= 2) rot_index = 0;
	}
};

// фигура в виде "J"
class J_figure : public figure{
	
	glm::vec2 rot_positions[4][4];
	int rot_index;
	
public:
	J_figure() : figure(){
		for(int i = 0; i < 4; i++){
			elem_array[i]->color = glm::vec4(.88f, .37f, .16f, 1.f);
		}
		
		rot_positions[0][0] = glm::vec2(0.f, 0.f);
		rot_positions[0][1] = glm::vec2(20.f, 0.f);
		rot_positions[0][2] = glm::vec2(20.f, 20.f);
		rot_positions[0][3] = glm::vec2(20.f, 40.f);
		
		rot_positions[1][0] = glm::vec2(0.f, 0.f);
		rot_positions[1][1] = glm::vec2(0.f, 20.f);
		rot_positions[1][2] = glm::vec2(20.f, 0.f);
		rot_positions[1][3] = glm::vec2(40.f, 0.f);
		
		rot_positions[2][0] = glm::vec2(0.f, 0.f);
		rot_positions[2][1] = glm::vec2(0.f, 20.f);
		rot_positions[2][2] = glm::vec2(0.f, 40.f);
		rot_positions[2][3] = glm::vec2(20.f, 40.f);
		
		rot_positions[3][0] = glm::vec2(0.f, 0.f);
		rot_positions[3][1] = glm::vec2(0.f, 20.f);
		rot_positions[3][2] = glm::vec2(-20.f, 20.f);
		rot_positions[3][3] = glm::vec2(-40.f, 20.f);
		
		elem_array[0]->setPosition(glm::vec2(0.f, 0.f));
		elem_array[1]->setPosition(glm::vec2(20.f, 0.f));
		elem_array[2]->setPosition(glm::vec2(20.f, 20.f));
		elem_array[3]->setPosition(glm::vec2(20.f, 40.f));
		
		rot_index = 1;
	}
	
	void rotate(){
		
		elem_array[0]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][0]);
		elem_array[1]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][1]);
		elem_array[2]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][2]);
		elem_array[3]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][3]);
		
		rot_index++;
		if (rot_index >= 4) rot_index = 0;
	}
	
	void rotate_back(){
		rot_index -= 2;
		if (rot_index < 0){
			rot_index = 4 + rot_index;
		}
		
		elem_array[0]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][0]);
		elem_array[1]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][1]);
		elem_array[2]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][2]);
		elem_array[3]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][3]);
		
		rot_index++;
		if (rot_index >= 4) rot_index = 0;
	}
	
};

// фигура в виде "L"
class L_figure : public figure{
	
	glm::vec2 rot_positions[4][4];
	int rot_index;
	
public:
	L_figure() : figure(){
		for(int i = 0; i < 4; i++){
			elem_array[i]->color = glm::vec4(.37f, .84f, .08f, 1.f);
		}
		
		rot_positions[0][0] = glm::vec2(0.f, 0.f);
		rot_positions[0][1] = glm::vec2(0.f, 20.f);
		rot_positions[0][2] = glm::vec2(0.f, 40.f);
		rot_positions[0][3] = glm::vec2(20.f, 0.f);
		
		rot_positions[1][0] = glm::vec2(0.f, 0.f);
		rot_positions[1][1] = glm::vec2(20.f, 0.f);
		rot_positions[1][2] = glm::vec2(40.f, 0.f);
		rot_positions[1][3] = glm::vec2(0.f, -20.f);
		
		rot_positions[2][0] = glm::vec2(0.f, 0.f);
		rot_positions[2][1] = glm::vec2(0.f, 20.f);
		rot_positions[2][2] = glm::vec2(0.f, 40.f);
		rot_positions[2][3] = glm::vec2(-20.f, 40.f);
		
		rot_positions[3][0] = glm::vec2(0.f, 0.f);
		rot_positions[3][1] = glm::vec2(0.f, 20.f);
		rot_positions[3][2] = glm::vec2(-20.f, 0.f);
		rot_positions[3][3] = glm::vec2(-40.f, 0.f);
		
		elem_array[0]->setPosition(rot_positions[0][0]);
		elem_array[1]->setPosition(rot_positions[0][1]);
		elem_array[2]->setPosition(rot_positions[0][2]);
		elem_array[3]->setPosition(rot_positions[0][3]);
		
		rot_index = 1;
	}
	
	void rotate(){
		
		elem_array[0]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][0]);
		elem_array[1]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][1]);
		elem_array[2]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][2]);
		elem_array[3]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][3]);
		
		rot_index++;
		if (rot_index >= 4) rot_index = 0;
	}
	
	void rotate_back(){
		rot_index -= 2;
		if (rot_index < 0){
			rot_index = 4 + rot_index;
		}
		
		elem_array[0]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][0]);
		elem_array[1]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][1]);
		elem_array[2]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][2]);
		elem_array[3]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][3]);
		
		rot_index++;
		if (rot_index >= 4) rot_index = 0;
	}
	
};

// фигура в виде "L"
class T_figure : public figure{
	
	glm::vec2 rot_positions[4][4];
	int rot_index;
	
public:
	T_figure() : figure(){
		for(int i = 0; i < 4; i++){
			elem_array[i]->color = glm::vec4(.08f, .84f, .53f, 1.f);
		}
		
		rot_positions[0][0] = glm::vec2(0.f, 0.f);
		rot_positions[0][1] = glm::vec2(0.f, 20.f);
		rot_positions[0][2] = glm::vec2(20.f, 20.f);
		rot_positions[0][3] = glm::vec2(-20.f, 20.f);
		
		rot_positions[1][0] = glm::vec2(0.f, 0.f);
		rot_positions[1][1] = glm::vec2(0.f, 20.f);
		rot_positions[1][2] = glm::vec2(0.f, 40.f);
		rot_positions[1][3] = glm::vec2(20.f, 20.f);
		
		rot_positions[2][0] = glm::vec2(0.f, 0.f);
		rot_positions[2][1] = glm::vec2(-20.f, 0.f);
		rot_positions[2][2] = glm::vec2(20.f, 0.f);
		rot_positions[2][3] = glm::vec2(0.f, 20.f);
		
		rot_positions[3][0] = glm::vec2(0.f, 0.f);
		rot_positions[3][1] = glm::vec2(0.f, 20.f);
		rot_positions[3][2] = glm::vec2(0.f, 40.f);
		rot_positions[3][3] = glm::vec2(-20.f, 20.f);
		
		elem_array[0]->setPosition(rot_positions[0][0]);
		elem_array[1]->setPosition(rot_positions[0][1]);
		elem_array[2]->setPosition(rot_positions[0][2]);
		elem_array[3]->setPosition(rot_positions[0][3]);
		
		rot_index = 1;
	}
	
	void rotate(){
		
		elem_array[0]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][0]);
		elem_array[1]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][1]);
		elem_array[2]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][2]);
		elem_array[3]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][3]);
		
		rot_index++;
		if (rot_index >= 4) rot_index = 0;
	}
	
	void rotate_back(){
		rot_index -= 2;
		if (rot_index < 0){
			rot_index = 4 + rot_index;
		}
		
		elem_array[0]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][0]);
		elem_array[1]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][1]);
		elem_array[2]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][2]);
		elem_array[3]->setPosition(elem_array[0]->getPosition() + rot_positions[rot_index][3]);
		
		rot_index++;
		if (rot_index >= 4) rot_index = 0;
	}
	
};


int main(){
	// инициализация glfw
	glfwInit();	
	
	// создание окна
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "window", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	// инициализация glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
    	cout<<"Failed to initialize GLEW"<<endl;
	}
	
	glViewport(0, 0, WIDTH, HEIGHT);
	
	//~~~~~VARIABLES~~~~~
	
	// массив разнообразных фигур
	figure *all_varieties[7] = {new O_figure(), new I_figure(), new J_figure(), new L_figure(), new S_figure(), new Z_figure(), new T_figure()};
	
	int score = 0;
	int score_speed_up = 0;
	
	// скорость движения объекта
	float speed = 1.f;
	figure *current_figure = all_varieties[GetRandomNumber(0, 6)];
	current_figure->setPosition(glm::vec2(0.f, 200.f));
	
	// массив элементов
	element *elements_matrix[10][20];
	// обнуление объектов в массиве элементов
	for(int i = 0; i < 10; i++)
		for(int j = 0; j < 20; j++){
			elements_matrix[i][j] = NULL;
		}
	enum direction {NONE, RIGHT, LEFT};
	
	bool oneClick = false;
	bool oneClickPause = false;
	bool pause = false;
	bool gameOver = false;
	
	// игровой цикл
	while(!glfwWindowShouldClose(window)){
		
		// проверка событий
		glfwPollEvents();
		
		if (gameOver){
			glfwSetWindowShouldClose(window, 1);
			cout<<"GaMe_OvEr!\n";
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::Q)){
			if (!oneClickPause){
				pause = !pause;
			}
			oneClickPause = true;
		} else oneClickPause = false;
		if (pause) continue;
		
		
		// отрисовка цвета заднего фона
		glClearColor(0.01f, 0.05f, 0.04f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// отрисовка объектов матрицы
		for(int i = 0; i < 10; i++)
			for(int j = 0; j < 20; j++){
				if (elements_matrix[i][j] != NULL){
					elements_matrix[i][j]->draw();
				}
			}
		if (score >= 5)
			speed = 1.0f / (score / 5);
		
		// действие по нажатию клавиши A
		if (sf::Keyboard::isKeyPressed(Keyboard::A)){
			if(!oneClick){
				// здесь проверка на пересечение края карты
				bool isAlowedMoving = true;
				for (int i = 0; i < 4; i++){
					int position_x = (int)(10 - ((WIDTH / 2) - current_figure->elem_array[i]->getPosition().x) / 20);
					int position_y = (int)(((HEIGHT / 2) - current_figure->elem_array[i]->getPosition().y) / 20) - 1;
					if (position_y >= 0){
						if ((position_x <= 0) || (elements_matrix[position_x - 1][position_y] != NULL))
							isAlowedMoving = false;
					}
					else{
						if ((position_x <= 0))
							isAlowedMoving = false;
					}
				}
				if (isAlowedMoving)
					current_figure->move(glm::vec2(-20.f, 0.f));
				oneClick = true;
			}
		}
		// действие по нажатию клавиши D
		else if (sf::Keyboard::isKeyPressed(Keyboard::D)){
			if(!oneClick){
				// здесь проверка на пересечение края карты
				bool isAlowedMoving = true;
				for (int i = 0; i < 4; i++){
					int position_x = (int)(10 - ((WIDTH / 2) - current_figure->elem_array[i]->getPosition().x) / 20);
					int position_y = (int)(((HEIGHT / 2) - current_figure->elem_array[i]->getPosition().y) / 20) - 1;
					if (position_y >= 0){
						if((position_x >= 9) || (elements_matrix[position_x + 1][position_y] != NULL))
							isAlowedMoving = false;
					}
					else{
						if (position_x >= 9)
							isAlowedMoving = false;
					}
				}
				if (isAlowedMoving)
					current_figure->move(glm::vec2(20.f, 0.f));
				oneClick = true;
			}
		}
		// действие по нажатию клавиши E
		else if (sf::Keyboard::isKeyPressed(Keyboard::E)){
			if(!oneClick){
				glm::vec2 pos = current_figure->getPosition();
				current_figure->rotate();
				// смещение позиции фигуры в случае пересечения с краем карты
				bool isInDiapasone = true;
				
				do{
					isInDiapasone = true;
					for (int i = 0; i < 4; i++){
						int position_x = (int)(10 - ((WIDTH / 2) - current_figure->elem_array[i]->getPosition().x) / 20);
						int position_y = (int)(((HEIGHT / 2) - current_figure->elem_array[i]->getPosition().y) / 20) - 1;
						if(position_x < 0){
							isInDiapasone = false;
							current_figure->move(glm::vec2(20.f, 0.f));
						}
						if(position_x > 9){
							isInDiapasone = false;
							current_figure->move(glm::vec2(-20.f, 0.f));
						}
					}
				} while(!isInDiapasone);
				
				// проверка на возможность поворота текущней фигуры между фигурами
				direction dir = NONE;
				bool returnPos = false;
				bool intersects = false;
				do{
					intersects = false;
					for (int i = 0; i < 4; i++){
						
						int position_x = (int)(10 - ((WIDTH / 2) - current_figure->elem_array[i]->getPosition().x) / 20);
						int position_y = (int)(((HEIGHT / 2) - current_figure->elem_array[i]->getPosition().y) / 20) - 1;
						
						if (elements_matrix[position_x][position_y] != NULL || ((position_x < 0) || (position_x > 9))){
							
							if (pos.x + 1.f < position_x){
								if (dir == LEFT){
									returnPos = true;
									goto label;
								}
								dir = RIGHT;
								current_figure->move(glm::vec2(-20.f, 0.f));
							}
							else {
								if (dir == RIGHT){
									returnPos = true;
									goto label;
								}
								dir = LEFT;
								current_figure->move(glm::vec2(20.f, 0.f));
							}
							
							intersects = true;
							break;
						}
					}
				} while (intersects);
				
				label:
				if (returnPos) {
					current_figure->rotate_back();
					current_figure->setPosition(pos);
				}
				
				oneClick = true;
			}
		}
		else if (sf::Keyboard::isKeyPressed(Keyboard::W)){
			bool isTouch;
			if(!oneClick){
				do{
					isTouch = true;
					int position_x, position_y;
					for (int i = 0; i < 4; i++){
						
						position_x = (int)(10 - ((WIDTH / 2) - current_figure->elem_array[i]->getPosition().x) / 20);
						position_y = (int)(((HEIGHT / 2) - current_figure->elem_array[i]->getPosition().y) / 20) - 1;
						
						if(position_y >= 0){
							if ((position_y >= 19) || (elements_matrix[position_x][position_y + 1] != NULL)){						
								isTouch = false;
								glfwSetTime(speed);
							}
						}
					}
					if (isTouch) current_figure->move(glm::vec2(0.f, -20.f));
				} while (isTouch);
				oneClick = true;
			}
		}
		else oneClick = false;
		
		// действие по нажатию клавиши S
		
		
		if (glfwGetTime() >= (sf::Keyboard::isKeyPressed(Keyboard::S) ? 0.05f : speed)){
			glfwSetTime(0.0f);
			
			
			// проверка на столкновение с другой фигурой или краем видимой области
			bool bump = false;
			
			int position_x, position_y;
			for (int i = 0; i < 4; i++){
				position_x = (int)(10 - ((WIDTH / 2) - current_figure->elem_array[i]->getPosition().x) / 20);
				position_y = (int)(((HEIGHT / 2) - current_figure->elem_array[i]->getPosition().y) / 20) - 1;
				
				if (position_y >= 0){
					if ((position_y >= 19) || (elements_matrix[position_x][position_y + 1] != NULL)){
						bump = true;
						if (position_y == 0) gameOver = true;
					}
				}
			}
			
			// сдвинуть, если нет столкновения
			if (!bump){
				current_figure->move(glm::vec2(.0f, -20.f));
			}
			// добавить в матрицу элементы и обновить текущую фигуру в случае столкновения
			else{
				for (int i = 0; i < 4; i++){
					position_x = (int)(10 - ((WIDTH / 2) - current_figure->elem_array[i]->getPosition().x) / 20);
					position_y = (int)(((HEIGHT / 2) - current_figure->elem_array[i]->getPosition().y) / 20) - 1;
					elements_matrix[position_x][position_y] = new element(*current_figure->elem_array[i]);
				}
				current_figure = all_varieties[GetRandomNumber(0, 6)];
				current_figure->setPosition(glm::vec2(0.f, 200.f));
				bump = false;
				
				// проверка на полный ряд и удаление такого
				bool lineIsFull = true; // полный ли ряд
				// проверка на полноту строки
				for(int i = 19; i >= 0; i--){
					lineIsFull = true;
					for(int j = 0; j < 10; j++){
						if(elements_matrix[j][i] == NULL){
							lineIsFull = false;
						}
					}
					// удаление полной строки и смещение 
					if(!lineIsFull) continue;
					else{
						score++;
						for(int j = i; j > 0; j--){
							for(int m = 0; m < 10; m++){
								elements_matrix[m][j] = NULL;
								if (elements_matrix[m][j - 1] != NULL){
									delete elements_matrix[m][j];
									elements_matrix[m][j] = elements_matrix[m][j - 1];
									elements_matrix[m][j]->move(glm::vec2(0.f, -20.f));
								}
							}
						}
						i++;
					}
				}
			}
		}
		// отрисовка текущей фигуры
		current_figure->draw();
		
		glfwSwapBuffers(window);
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}



















