#ifndef GLMEMORYBUFFERS_HPP
#define GLMEMORYBUFFERS_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <map>



//
//
// class for buffer objects: VBOs, EBOs, etc

class ngnBufferObject {
	
public:
	
	// value/location
	GLuint val;
	
	// type of buffer (GL_ARRAY_BUFFER || GL_ELEMENT_ARRAY_BUFFER, etc)
	GLenum type;
	
	// default constructor and initialize with type
	ngnBufferObject(GLenum type = GL_ARRAY_BUFFER) : type(type) {}
	
	// generate object
	void generate() {
		
		glGenBuffers(1, &val);
	}
	
	// bind object
	void bind() {
		
		glBindBuffer(type, val);
	}
	
	// glBufferData
	template<typename T>
	void ngnPopulateBuffer(GLuint nElems, T* data, GLenum usage) {
		
		glBufferData(type, noElements * sizeof(T), data, usage);
	}
	
	// glBufferSubData ???
	template<typename T>
	void ngnUpdateBuffer(GLintptr offset, GLuint nElems, T* data) {
		
		glBufferSubData(type, offset, nElems * sizeof(T), data);
	}
	
	// glVertexAttribPointer
	template<typename T>
	void sgVertexAttribPointer(GLuint idx, GLint size, GLenum type, GLuint stride, GLuint offset, GLuint divisor = 0) {
		
		glVertexAttribPointer(idx, size, type, GL_FALSE, stride * sizeof(T), (void*)(offset * sizeof(T)));
		glEnableVertexAttribArray(idx);
		
		if (divisor > 0) {
			
			// reset @idx attribute every @divisor iteration (instancing)
			glVertexAttribDivisor(idx, divisor);
		}
	}
	
	// clear
	void clear() {
		
		glBindBuffer(type, 0);
	}
	
	// cleanUp
	void cleanUp() {
		
		glDeleteBuffers(1, &val);
	}
};



//
//
// class for array objects - VAO

class ngnArrayObject {

public:
	
	// value/location
	GLuint val;
	
	// map of names to buffers
	std::map<const char*, ngnBufferObject> buffers;
	
	// get buffer (override [])
	ngnBufferObject& operator[](const char* key) {
		
		return buffers[key];
	}
	
	// generate object
	void generate() {
		
		glGenVertexArrays(1, &val);
	}
	
	// bind
	void bind() {
		
		glBindVertexArray(val);
	}
	
	// draw arrays
	void draw(GLenum mode, GLuint first, GLuint count) {
		
		glDrawArrays(mode, first, count);
	}
	
	// draw
	void draw(GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instanceCount = 1) {
		
		glDrawElementsInstanced(mode, count, type, (void*)indices, instanceCount);
	}
	
	// cleanUp
	void cleanUp() {
		
		glDeleteVertexArrays(1, &val);
		
		for (auto& pair : buffers) {
			
			pair.second.cleanUp();
		}
	}
	
	// clear    
	static void clear() {
		
		glBindVertexArray(0);
	}
};

#endif
