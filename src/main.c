#define GLFW_INCLUDE_GLU
//#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static void error_callback(int error, const char* description){
  fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

GLuint loadShader(GLenum type, char *shaderSrc){
  GLuint shader;
  GLint compiled;

  shader = glCreateShader(type);
  if(shader == 0)
    return 0;
  
  printf("Shader Created\n");  
  printf("Shader Source Loading \n%s\n", shaderSrc);
  glShaderSource(shader, 1, shaderSrc, NULL);
  printf("Shader Source Loaded\n");

  glCompileShader(shader);
  printf("Shader Compiled\n");

  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  if(!compiled){
    printf("FUCK IT ALL!!!!****\n\n");
    GLint infoLen = 0;; 
    
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    if(infoLen > 0){
      char* infoLog = malloc(sizeof(char)* infoLen);
      glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
      printf("ERROR: %s\n", infoLog);
      free(infoLog);
    }
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

GLuint loadShaderFile(GLenum type, const char *shaderPath){
  FILE *fp;
  char *source;
  GLuint shader = 0;

  if((fp = fopen(shaderPath, "r"))==NULL) {
    printf("Cannot open shader file %s.\n", shaderPath);
    exit(1);
  }

  fseek(fp, 0L, SEEK_END);
  int sz = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  source = calloc(sz, sizeof(char));

  //if(
  fread(source, 1, sz, fp);//)
  //printf("%s", source);

  fclose(fp);
  free(source);
  return shader;
}

int Init(){
  GLint vectshad1 = loadShaderFile(GL_VERTEX_SHADER, "bin/julia.vert");
  GLint fragshad1 = loadShaderFile(GL_FRAGMENT_SHADER, "bin/julia.frag");
  GLint progid = glCreateProgram();
  GLint linked;
  
  if(progid == 0){
    printf("Error creating GL Program object.\n");
    exit(1);
  }

  glAttachShader(progid, vectshad1);
  glAttachShader(progid, fragshad1);

  glBindAttribLocation(progid, 0, "vPosition");

  glLinkProgram(progid);

  glGetProgramiv(progid, GL_LINK_STATUS, &linked);

  if(!linked){
    printf("FUCK IT ALL!!!!****\n\n");
    GLint infoLen = 0;; 
    
    glGetProgramiv(progid, GL_INFO_LOG_LENGTH, &infoLen);
    if(infoLen > 0){
      char* infoLog = malloc(sizeof(char)* infoLen);
      glGetProgramInfoLog(progid, infoLen, NULL, infoLog);
      printf("ERROR: %s\n", infoLog);
      free(infoLog);
    }
    glDeleteProgram(progid);
    return 0;
  }
  
  glClearColor(0.5, 0.5, 0.5, 1.0);
  //printf("ID: %d\n", progid);
  return progid;
}

int main(int argc, char* argv[]){
  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);

  GLint progid = Init();

  GLfloat vVertices[] = {0.0f, 0.5f, 0.0f,
                         -0.5f, -0.5f, 0.0f,
                         0.5f, -0.5f, 0.0f};

  while (!glfwWindowShouldClose(window))
    {
      float ratio;
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);
      ratio = width / (float) height;
      glViewport(0, 0, width, height);
      glClear(GL_COLOR_BUFFER_BIT);

      glUseProgram(progid);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
      glEnableVertexAttribArray(0);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
      glBegin(GL_TRIANGLES);
      glColor3f(1.f, 0.f, 0.f);
      glVertex3f(-0.6f, -0.4f, 0.f);
      glColor3f(0.f, 1.f, 0.f);
      glVertex3f(0.6f, -0.4f, 0.f);
      glColor3f(0.f, 0.f, 1.f);
      glVertex3f(0.f, 0.6f, 0.f);
      glEnd();

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
