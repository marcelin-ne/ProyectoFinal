//INCLUIMOS LAS DIFERNTES LIBRER�AS
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>

#include <iostream>

//-------------------------CONFIGURAMOS LA VENTANA----------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos); //se dice que la vista de la pantalla se mover seg�n el mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); // Zoom en la ventana 
void processInput(GLFWwindow* window);

// Configuramos las dimenciones de la ventana
const unsigned int SCR_WIDTH = 1200; 
const unsigned int SCR_HEIGHT = 600; 

// -------------------------- CAMERA -----------------------------------------------------
Camera camera(glm::vec3(0.0f, 0.0f, 9.0f)); // Declaramos el Vector Camara
// Centro de Pantalla 
float lastX = SCR_WIDTH / 2.0f;  
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true; 

// Velocidad de Camra 
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

float VelocidadRota = 4.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Marcela Montalvo Prueba IIB 1727277418", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    
    //Se hace llamado al Shader clases y a cada una de sus clases (VertexShader y FragmentShader)
    Shader ourShader("shaders/vertexShader.vs", "shaders/fragmentShader.fs");

    //Vertices de los cubos 
    float vertices[] = {
        //Coord Posicion , Texture Coord
        //Cara atras
         -0.5f, -0.5f, -0.5f, 0.60f, 0.57f, 
         0.5f, -0.5f, -0.5f,  0.60, 0.43f,  
         0.5f,  0.5f, -0.5f,  0.73f, 0.43f, 
         0.5f,  0.5f, -0.5f,  0.73f, 0.43f, 
        -0.5f,  0.5f, -0.5f,  0.73f, 0.57f, 
        -0.5f, -0.5f, -0.5f,  0.60f, 0.57f, 

        //Cara adelante
        -0.5f, -0.5f,  0.5f,  0.32f, 0.57f, 
         0.5f, -0.5f,  0.5f,  0.32f, 0.43f, 
         0.5f,  0.5f,  0.5f,  0.45f, 0.43f, 
         0.5f,  0.5f,  0.5f,  0.45f, 0.43f, 
        -0.5f,  0.5f,  0.5f,  0.45f, 0.57f, 
        -0.5f, -0.5f,  0.5f,  0.32f, 0.57f, 

        //Cara Izquierda
        -0.5f,  0.5f,  0.5f,  0.45f, 0.57f, 
        -0.5f,  0.5f, -0.5f,  0.45f, 0.43f, 
        -0.5f, -0.5f, -0.5f,  0.60f, 0.43f, 
        -0.5f, -0.5f, -0.5f,  0.60f, 0.43f, 
        -0.5f, -0.5f,  0.5f,  0.60f, 0.57f, 
        -0.5f,  0.5f,  0.5f,  0.45f, 0.57f, 

        //Cara Derecha 
         0.5f,  0.5f,  0.5f,  0.17f, 0.57f, 
         0.5f,  0.5f, -0.5f,  0.17f, 0.43f, 
         0.5f, -0.5f, -0.5f,  0.30f, 0.43f,  
         0.5f, -0.5f, -0.5f,  0.30f, 0.43f,  
         0.5f, -0.5f,  0.5f,  0.30f, 0.57f, 
         0.5f,  0.5f,  0.5f,  0.17f, 0.57f, 

         //Cara Abajo 
         -0.5f, -0.5f, -0.5f,  0.31f, 0.37f, 
         0.5f, -0.5f, -0.5f,  0.31f, 0.27f, 
         0.5f, -0.5f,  0.5f,  0.45f, 0.27f, 
         0.5f, -0.5f,  0.5f,  0.45f, 0.27f, 
        -0.5f, -0.5f,  0.5f,  0.45f, 0.37f, 
        -0.5f, -0.5f, -0.5f,  0.31f, 0.37f, 
       
        //Cara Arriba 
        -0.5f,  0.5f, -0.5f,  0.31f, 0.71f, 
         0.5f,  0.5f, -0.5f,  0.31f, 0.57f, 
         0.5f,  0.5f,  0.5f,  0.45f, 0.57f, 
         0.5f,  0.5f,  0.5f,  0.45f, 0.57f, 
        -0.5f,  0.5f,  0.5f,  0.47f, 0.71f, 
        -0.5f,  0.5f, -0.5f,  0.31f, 0.71f, 
        
        
     
    };
   
    // SEGUNDO CUBO 
   float vertices2[] = {

      //Coord Posicion , Texture Coord
        //Cara atras
         -0.625f, -0.625f, -0.625f, 0.18f, 0.44f, 
         0.625f, -0.625f, -0.625f,  0.18f, 0.27f,  
         0.625f,  0.625f, -0.625f,  0.37f, 0.27f, 
         0.625f,  0.625f, -0.625f,  0.37f, 0.27f, 
        -0.625f,  0.625f, -0.625f,  0.37f, 0.44f, 
        -0.625f, -0.625f, -0.625f,  0.18f, 0.44f, 

        //Cara adelante
         -0.625f, -0.625f,  0.625f,  0.44f, 0.44f, 
          0.625f, -0.625f,  0.625f,  0.44f, 0.28f, 
          0.625f,  0.625f,  0.625f,  0.80f, 0.28f, 
          0.625f,  0.625f,  0.625f,  0.80f, 0.28f, 
         -0.625f,  0.625f,  0.625f,  0.80f, 0.44f,
         -0.625f, -0.625f,  0.625f,  0.44f, 0.44f, 


        //Cara Izquierda 
       -0.625f, -0.625f, -0.625f,  0.36f, 0.44f, 
          0.625f, -0.625f, -0.625f,  0.36f, 0.27f, 
          0.625f, -0.625f,  0.625f,  0.555f, 0.27f, 
          0.625f, -0.625f,  0.625f,  0.555f, 0.27f, 
         -0.625f, -0.625f,  0.625f,  0.555f, 0.44f,
         -0.625f, -0.625f, -0.625f,  0.36f, 0.44f, 

        //Cara Arriba
         -0.625f,  0.625f, -0.625f,  0.57f, 0.27f, 
         0.625f,  0.625f, -0.625f,  0.57f, 0.04f, 
         0.625f,  0.625f,  0.625f,  0.75f, 0.04f, 
         0.625f,  0.625f,  0.625f,  0.75f, 0.04f, 
        -0.625f,  0.625f,  0.625f,  0.75f, 0.27f, 
        -0.625f,  0.625f, -0.625f,  0.57f, 0.27f, 

    
         //Cara Abajo  
        -0.625f,  0.625f,  0.625f,  0.57f, 0.66f, 
        -0.625f,  0.625f, -0.625f,  0.57f, 0.45f, 
        -0.625f, -0.625f, -0.625f,  0.75f, 0.45f, 
        -0.625f, -0.625f, -0.625f,  0.75f, 0.45f, 
        -0.625f, -0.625f,  0.625f,  0.75f, 0.66f, 
        -0.625f,  0.625f,  0.625f,  0.57f, 0.66f, 

       
        //Cara Derecha 
        -0.625f,  0.625f, -0.625f,  0.75f, 0.44f, 
         0.625f,  0.625f, -0.625f,  0.75f, 0.28f, 
         0.625f,  0.625f,  0.625f,  0.95f, 0.28f, 
         0.625f,  0.625f,  0.625f,  0.95f, 0.28f, 
        -0.625f,  0.625f,  0.625f,  0.95f, 0.44f, 
        -0.625f,  0.625f, -0.625f,  0.75f, 0.44f, 
        
        
    };

    // world space positions del primero cubo // En Centro de Coordenadas 
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
    };

    // world space positions del segundo cubo ; 
        glm::vec3 cubePositions2[] = {
        glm::vec3(1.5f,  0.5f,  2.5f),
        glm::vec3(2.5f,  -3.0f, -10.5f),
      
    };

    //declaraciones de los diferntes objetos para el primer cubo y para el seegundo cubo
    unsigned int VBO,VBO1, VAO,VAO1;
    //Primer Cubo
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //-------------------------------------CUBO 2 ------------------------------------------------
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //-----------------------------------------------------------------------------------------------


    // Cargar y Crear texturas
    // Declaramos los objects
    unsigned int texture1, texture2;

    // texture 1

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("textures/Bloque_1.png", &width, &height, &nrChannels, 0);    if (data)
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("textures/Texture2.png", &width, &height, &nrChannels, 0);    if (data)
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
    stbi_image_free(data);

    // Creamos las reaciones entre las units y las Texturas
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
       // configuramos la velocidad
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.9f, 0.9f, 0.5f, 1.0f);



        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activamos las texturas con sus units correspondientes
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // activamos el shader
        ourShader.use();

        // Se pasa la matriz de projecci�n al Vertex Shader

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);
        //------ Cubo Textura1 ------------
        // render boxes
        glBindVertexArray(VAO);


     

             glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[0]);
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

        // -------------cubo 2 --------------

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture2);
        // activate shader
        
        ourShader.use();

        // render boxes
        glBindVertexArray(VAO1);


        /* C2 -->Girar en sentido antihorario y con eje de rotación en X */
        
            // calculate the model matrix for each object and pass it to shader before drawing
            model = glm::translate(model, cubePositions2[0]);
            float angle = glfwGetTime() * VelocidadRota;
            model = glm::rotate(model, glm::radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
           
            //C3 -->Trasladar en forma circular en plano YZ 

            float angle1 = 100.0f * glfwGetTime();
            model = glm::rotate(model, glm::radians(angle1), glm::vec3(1.0f, 0.0f, 1.0f));
            model = glm::translate(model, cubePositions2[1]);
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
           
       

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Liberamos Los VAO's y VBO's 
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);

    
    glfwTerminate();
    return 0;
}

// Configuramos las acciones por teclado
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
        VelocidadRota = VelocidadRota + 0.25f;
    };
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
        VelocidadRota = VelocidadRota - 0.25f;
    };
    

    
        
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


//DETERMINA EL MOVIENTO DEL MOUSE
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

//Scroll
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}