#include "openglBackend.h"
namespace Default_Values
{
	
	// settings
	constexpr unsigned int SCR_WIDTH = 1920;
	constexpr unsigned int SCR_HEIGHT = 1080;

	// movement
	float deltaTime{ 0.0f };
	float lastTime{ 0.0f };

	bool firstMove{ true };
	float lastX{ SCR_WIDTH / 2.0f };
	float lastY{ SCR_HEIGHT / 2.0f };

	glm::vec3 lightPos = glm::vec3(0.0f, 1.0f, 3.0f);
	
	// camera
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

	// cursor
	extern bool cursorState{ false };
}


namespace OpenGL_Backend
{
	GLuint g_vertexArray{ 0 };
	GLuint g_vertexBuffer{ 0 };
	GLuint g_elementBuffer{ 0 };

	GLuint getVertexVAO()
	{
		return g_vertexArray;
	}
	GLuint getVertexVBO()
	{
		return g_vertexBuffer;
	}
	GLuint getElementBuffer()
	{
		return g_elementBuffer;
	}


	int initialization()
	{	
		// glfw: initialize and configure
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
		// glfw window creation
	// --------------------
		GLFWwindow* window = glfwCreateWindow(Default_Values::SCR_WIDTH,
			Default_Values::SCR_HEIGHT, "LimboEngine", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetKeyCallback(window, processInputMode_callback);
		glfwSetCursorPosCallback(window, mouse_callback);


		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
	}

	void drawUnskinnedByVBO(std::vector<float>& vertices)
	{
		glGenBuffers(1, &g_vertexBuffer);

		glGenVertexArrays(1, &g_vertexArray);
		glBindVertexArray(g_vertexArray);

		glBindBuffer(GL_ARRAY_BUFFER, g_vertexBuffer);


		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(0);
	}
	
	void drawUnskinnedByEBO(std::vector<float>& vertices, std::vector<uint32_t>& indices)
	{
		glGenBuffers(1, &g_vertexBuffer);
		glGenBuffers(1, &g_elementBuffer);

		glGenVertexArrays(1, &g_vertexArray);
		glBindVertexArray(g_vertexArray);

		glBindBuffer(GL_ARRAY_BUFFER, g_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);


		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(float), (void*)0);

	}

	void deleteGlData()
	{
		glDeleteVertexArrays(1, &g_vertexArray);
		glDeleteBuffers(1, &g_vertexBuffer);
		glDeleteBuffers(1, &g_elementBuffer);


		glfwTerminate();
	}


	void APIENTRY glDebugOutput(GLenum source,
		GLenum type,
		unsigned int id,
		GLenum severity,
		GLsizei length,
		const char* message,
		const void* userParam)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::cout << "---------------" << std::endl;
		std::cout << "Debug message (" << id << "): " << message << std::endl;

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
		} std::cout << std::endl;

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
		} std::cout << std::endl;

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
		} std::cout << std::endl;
		std::cout << std::endl;
	}


	///// CALLBACKS ///////

	void mouse_callback(GLFWwindow* window, double xPos, double yPos)
	{
		if (Default_Values::firstMove) {
			Default_Values::lastX = xPos;
			Default_Values::lastY = yPos;
			Default_Values::firstMove = false;
		}
		// offset - AMOUNT of movement in 1 flick
		float xOffset = xPos - Default_Values::lastX;
		float yOffset = Default_Values::lastY - yPos;
		Default_Values::lastX = xPos;
		Default_Values::lastY = yPos;

		Default_Values::camera.processMouse(xOffset, yOffset);


	}
	void processInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);


		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			Default_Values::camera.processKeyboard(CameraMovement::forward, Default_Values::deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			Default_Values::camera.processKeyboard(CameraMovement::backward, Default_Values::deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			Default_Values::camera.processKeyboard(CameraMovement::left, Default_Values::deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			Default_Values::camera.processKeyboard(CameraMovement::right, Default_Values::	deltaTime);




	}

	void lightMove(GLFWwindow* window)
	{
		const float speed = 2.5f * Default_Values::deltaTime;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			Default_Values::lightPos.x -= speed;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			Default_Values::lightPos.x += speed;
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			Default_Values::lightPos.z -= speed;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			Default_Values::lightPos.z += speed;
		}
	}

	void processInputMode_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
		{
			cursorState = !cursorState;
		}


		if (cursorState)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}
