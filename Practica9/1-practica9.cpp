/*
Semestre 2024-1
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transforomación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Adicional.- ,Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
// Variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;


//----animacion del letrero 
float CartelF;
float Cartel_Rotacion;
float TiempoP;
float AnguloR;
bool avanza;
bool Inicio_Cartel;
bool Pausar;
bool Bajando;
bool Esperar_Arriba;
bool Esperar_Abajo;

//ANIMACION DEL DADO
float girdadoX;
float girdadoY;
float girdadoZ;
float rotprogresivaX;
float rotprogresivaY;
float rotprogresivaZ;
float rotprogresivaOffSet;
bool obtenerValoresAleatorios;
bool reaTodosLosMovimientos;
bool cayendo = true;

GLfloat arrancar;
GLfloat Animacion_Cartel;
GLfloat Arrancar_globo;



Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

//dado de 10 caras
Texture dado10Caras;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

//------MODELO DE ARCO--------
Model base_arco;
Model arco;

//----MODEL DEL GLOBO
Model globo;
Model moto;
Model llantam;
Model tablero;



Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

//dado de 10 caras
void CrearDado10C()
{
	// clang-format off
	unsigned int cubo_indices_10[] = {
		// superior 01
		0, 1, 2,

		// superior 02
		3 ,4, 5,

		// superior 03
		6, 7, 8,

		// superior 04
		9, 10, 11,

		// superior 05
		12, 13, 14,

		// "inferior"

		//inferior 01
		15, 16, 17,

		//inferior 02
		18, 19 ,20,

		//inferior 03
		21, 22, 23,

		//inferior 04
		24, 25, 26,

		//inferior 05
		27, 28, 29,

	};

	GLfloat cubo_vertices_10[] = {


		//----superior-------

		//superior 1
		//x		y		z		S		T			NX		NY		NZ
		-0.3f, 0.0f,  0.5f,		0.495f,	0.935f,		0.0f,	0.0f,	-1.0f,
		0.3f, 0.0f,  0.5f,	    0.28f,  0.87f,		0.0f,	0.0f,	-1.0f,
		0.0f,  0.45f,  1.0f,	0.48f,	0.72f,		0.0f,	0.0f,	-1.0f,

		//superior 2
		//x		y		z		S		T			NX		NY		NZ
		0.3f, 0.0f,  0.5f,		0.638f,  0.347f,	0.0f,	0.0f,	-1.0f,
		0.45f, 0.6f,  0.5f,		0.49f,	0.478f,		0.0f,	0.0f,	-1.0f,
		0.0f,  0.45f,  1.0f,	0.383f,	0.284f,		0.0f,	0.0f,	-1.0f,

		//superior 3
		//x		y		z		S		T			NX		NY		NZ
		0.45f, 0.6f,  0.5f,		0.371f,	0.533f,		0.0f,	0.0f,	-1.0f,
		0.0f, 0.95f,  0.5f,		0.602f,  0.528f,	0.0f,	0.0f,	-1.0f,
		0.0f,  0.45f,  1.0f,	0.478f,	0.729f,		0.0f,	0.0f,	-1.0f,

		//superior 4
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.95f,  0.5f,		0.417f,  0.074f,	0.0f,	0.0f,	-1.0f,
		-0.45f, 0.6f,  0.5f,	0.607f,	0.168f,		0.0f,	0.0f,	-1.0f,
		0.0f,  0.45f,  1.0f,	0.383f,	0.284f,		0.0f,	0.0f,	-1.0f,

		//superior 5
		//x		y		z		S		T			NX		NY		NZ
		-0.45f, 0.6f,  0.5f,	0.741f,	0.675f,		0.0f,	0.0f,	-1.0f,
		-0.3f, 0.0f,  0.5f,		0.695f,  0.852f,	0.0f,	0.0f,	-1.0f,
		0.0f,  0.45f,  1.0f,	0.478f,	0.725f,		0.0f,	0.0f,	-1.0f,

		//----inferiores---------

		//inferior 1
		//x		y		z		S		T			NX		NY		NZ
		-0.3f, 0.0f,  0.5f,		0.609f,	0.170f,		0.0f,	0.0f,	-1.0f,
		0.3f, 0.0f,  0.5f,		0.639f,  0.345f,	0.0f,	0.0f,	-1.0f,
		0.0f,  0.45f,  0.0f,	0.388f,	0.284f,		0.0f,	0.0f,	-1.0f,

		//inferior 2
		//x		y		z		S		T			NX		NY		NZ
		0.3f, 0.0f,  0.5f,		0.484f,  0.479f,	0.0f,	0.0f,	-1.0f,
		0.45f, 0.6f,  0.5f,		0.263f,	0.473f,		0.0f,	0.0f,	-1.0f,
		0.0f,  0.45f,  0.0f,	0.383f,	0.291f,		0.0f,	0.0f,	-1.0f,

		//inferior 3
		//x		y		z		S		T			NX		NY		NZ
		0.45f, 0.6f,  0.5f,		0.695f,	0.853f,		0.0f,	0.0f,	-1.0f,
		0.0f, 0.95f,  0.5f,		0.497f,  0.938f,	0.0f,	0.0f,	-1.0f,
		0.0f,  0.45f,  0.0f,	0.479f,	0.726f,		0.0f,	0.0f,	-1.0f,

		//inferior 4
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.95f,  0.5f,		0.201f,  0.127f,	0.0f,	0.0f,	-1.0f,
		-0.45f, 0.6f,  0.5f,	0.417f,	0.074f,		0.0f,	0.0f,	-1.0f,
		0.0f,  0.45f,  0.0f,	0.382f,	0.284f,		0.0f,	0.0f,	-1.0f,

		//inferior 5
		//x		y		z		S		T			NX		NY		NZ
		-0.45f, 0.6f,  0.5f,	0.6f,	0.535f,		0.0f,	0.0f,	-1.0f,
		-0.3f, 0.0f,  0.5f,		0.74f,  0.676f,		0.0f,	0.0f,	-1.0f,
		0.0f,  0.45f,  0.0f,	0.48f,	0.725f,		0.0f,	0.0f,	-1.0f,
	};

	Mesh* dado10 = new Mesh();
	dado10->CreateMesh(cubo_vertices_10, cubo_indices_10, 250, 50); //192,36
	meshList.push_back(dado10);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado10C();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	//DADO DE 10 CARAS
	dado10Caras = Texture("Textures/dado10caras.png");
	dado10Caras.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	//MODELO DE ARCO
	base_arco = Model();
	base_arco.LoadModel("Models/Base_arco.obj");

	arco = Model();
	arco.LoadModel("Models/cartelobj.obj");

	//modelo del globo------------------------
	globo = Model();
	globo.LoadModel("Models/globo.obj");

	tablero = Model();
	tablero.LoadModel("Models/tablero.obj");


	moto = Model();
	moto.LoadModel("Models/moto.obj");

	llantam = Model();
	llantam.LoadModel("Models/llanta.obj");



	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	// Variables
	movCoche = 0.0f;
	movOffset = 0.3f;
	rotllanta = 0.0f; // Origen de la llanta con repsecto aque inicia en 0 rotacion
	rotllantaOffset = 5.0f; // Velocidad de rotaccion
	avanza = true;

	//-----animacion del letrero 

	CartelF = 0.0f;
	Cartel_Rotacion = 0.1f;
	Inicio_Cartel = true;
	TiempoP = 0.0f;
	Pausar = false;
	Bajando = true;
	Esperar_Arriba = false;
	Esperar_Abajo = false;
	AnguloR = 0.0f;
	glfwGetTime();


	//ANIMACION DEL DADO
	girdadoX = 0.0f;
	girdadoY = 0.0f;
	girdadoZ = 0.0f;
	rotprogresivaOffSet = 20.0f;
	obtenerValoresAleatorios = true;
	reaTodosLosMovimientos = false;
	mainWindow.setEnciende(false);


	//animacion del globo
	GLfloat movgloboX = 2.0f; // Origen
	GLfloat movgloboZ = 5.5f; // Origen
	GLfloat movOffset_globo = 0.1f; // Velocidad del Helicoptero
	GLfloat globoRotation = 0.0f; // Para controlar la rotación
	int fase = 0; // Variable para controlar en qué fase del movimiento estamos
	bool avanza2 = true;
	float rot_llanta = 0.0f; // Origen
	float rot_llantaOffset = 90.0f; // Grados por segundo, giro de Aspas


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//-------------------------------------------------------

		//ANIMACION DEL CARRO
		arrancar = glfwGetTime();

		if (arrancar > 2)
		{
			if (avanza)
			{
				if (movCoche > -306.0f)
				{
					movCoche -= movOffset * deltaTime;
					//printf("avanza%f \n ",movCoche);
					rotllanta += rotllantaOffset * deltaTime;
				}
				else
				{
					avanza = !avanza;
				}
			}
			else
			{
				if (movCoche < 284.0f)
				{
					movCoche += movOffset * deltaTime;
					//printf("avanza%f \n ",movCoche);
					rotllanta -= rotllantaOffset * deltaTime;
				}
				else
				{
					avanza = !avanza;
				}
			}
		}

		//FINALIZACION DE ANIMACION DEL CARRO


		//----------------------------------------------
		

		//EMPIEZA ANIMACION DEL CARTEL
		Animacion_Cartel = glfwGetTime();

		if (Animacion_Cartel > 2)
		{
			if (!Pausar)
			{
				if (Bajando)
				{
					if (CartelF < 2.6f)//PARA QUE TENGA UN LIMITE DONDE TENGA QUE SUBIR 
					{
						CartelF += Cartel_Rotacion * deltaTime;
					}
					else
					{
						Pausar = true;
						TiempoP = Animacion_Cartel;
						Esperar_Arriba = true;
						AnguloR = 0.0f;
					}
				}
				else
				{
					if (CartelF > 0.3f)//PARA QUE TENGA UN LIMITE HASTA DONDE TIENE QUE BAJAR.
					{
						CartelF -= Cartel_Rotacion * deltaTime;
					}
					else
					{
						Pausar = true;
						TiempoP = Animacion_Cartel;
						Esperar_Abajo = true;
						AnguloR = 0.0f;
					}
				}
			}
			else
			{
				float tiempoTranscurrido = Animacion_Cartel - TiempoP;

				if (tiempoTranscurrido < 2.0f)
				{
					AnguloR = 180.0f * (tiempoTranscurrido / 2.0f);
				}
				else
				{
					AnguloR = 360.0f;
				}

				if (Esperar_Arriba && tiempoTranscurrido >= 2.0f)
				{
					Pausar = false;
					Esperar_Arriba = false;
					Bajando = false;
				}
				else if (Esperar_Abajo && tiempoTranscurrido >= 2.0f)
				{
					Pausar = false;
					Esperar_Abajo = false;
					Bajando = true;
				}
			}
		}

		//TERMINA ANIMACION DEL CARTEL

		//--------------------------------------------------------------


		//animacion del globo

		Arrancar_globo = glfwGetTime();

		if (Arrancar_globo > 2.0f)
		{
			// Mantén la rotación de las aspas activa durante todas las fases
			rot_llanta += rot_llantaOffset * deltaTime;


			switch (fase)
			{
				//case 0
			case 0:
				if (movgloboX > -23.0f)
				{
					movgloboX -= movOffset_globo * deltaTime;
				}
				else
				{
					fase = 1; // se cambia lo que es la rotacion de la fase
				}
				break;

				// se rota los 90 grados en el caso 1 
			case 1:
				if (globoRotation < 90.0f)
				{
					globoRotation += 90.0f * deltaTime; // rtoacion
				}
				else
				{
					globoRotation = 90.0f; 
					fase = 2; // se cambia la rotacion hacia el eje z
				}
				break;

				// case 2 
			case 2:
				if (movgloboZ < 27.0f)
				{
					movgloboZ += movOffset_globo * deltaTime;
				}
				else
				{
					fase = 3; // se rotacion
				}
				break;

				// case 3
			case 3:
				if (globoRotation < 180.0f)
				{
					globoRotation += 90.0f * deltaTime;
				}
				else
				{
					globoRotation = 180.0f;
					fase = 4; // movimiento en X
				}
				break;

				//case 4
			case 4:
				if (movgloboX < 3.0f)
				{
					movgloboX += movOffset_globo * deltaTime;
				}
				else
				{
					fase = 5; 
				}
				break;

				// case5
			case 5:
				if (globoRotation < 270.0f)
				{
					globoRotation += 90.0f * deltaTime;
				}
				else
				{
					globoRotation = 270.0f;
					fase = 6; 
				}
				break;

		
			case 6:
				if (movgloboZ > 5.0f)
				{
					movgloboZ -= movOffset_globo * deltaTime;
				}
				else
				{
					fase = 7; 
				}
				break;

				
			case 7:
				if (globoRotation < 360.0f)
				{
					globoRotation += 90.0f * deltaTime;
				}
				else
				{
					globoRotation = 0.0f; 
					fase = 0; 
				}
				break;
			}
		}

	





		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
			glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -1.0f, 3.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


		//Dado de 10 caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, 0.5f, 7.0f)); //4.5,9.5,-2.0
		model = glm::rotate(model, 90 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f)); //inclinaci n
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		/*Rotacion base del dado, donde permite estar acostado el dado*/
		model = glm::rotate(model, glm::radians(46.33f + rotprogresivaY), glm::vec3(0.0f, 0.0f, 1.0f));//ROTAMIENTO
		model = glm::rotate(model, glm::radians(46.33f + rotprogresivaY), glm::vec3(0.0f, 0.0f, 1.0f));//ROTAMIENTO
		model = glm::rotate(model, glm::radians(46.33f + rotprogresivaX), glm::vec3(1.0f, 0.0f, 0.0f));//ROTAMIENTO
	
		printf("", mainWindow.getEnciende1());//SE PRESIONA LA TECLA

		if (mainWindow.getEnciende1()) {
			if (obtenerValoresAleatorios) {
				girdadoX = (float)(180.0f * (rand() % 11));
				girdadoY = (float)(072.0f * (rand() % 11));
				girdadoZ = (float)(180.0f * (rand() % 11));
				rotprogresivaX = rotprogresivaY = rotprogresivaZ = 0.0f;
				reaTodosLosMovimientos = true;
				obtenerValoresAleatorios = false;
			}


			//ANIMACION
			if (rotprogresivaX < girdadoX && reaTodosLosMovimientos) {
				rotprogresivaX += rotprogresivaOffSet * deltaTime;
			}
			if (rotprogresivaY < girdadoY && reaTodosLosMovimientos) {
				rotprogresivaY += rotprogresivaOffSet * deltaTime;
			}
			if (rotprogresivaZ < girdadoZ && reaTodosLosMovimientos) {
				rotprogresivaZ += rotprogresivaOffSet * deltaTime;
			}

			if (!(rotprogresivaX < girdadoX) && !(rotprogresivaY < girdadoY) && !(rotprogresivaZ < girdadoZ)) {
				reaTodosLosMovimientos = false;
				obtenerValoresAleatorios = true;
				mainWindow.setEnciende(false);
			}
		}
		//TERMINA ANIMACION

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado10Caras.UseTexture();
		meshList[4]->RenderMesh();
		//FIN DEL DADO DE 10 CARAS


//--------------globo----------------------------


 
// 		   

		//SE CREA EL GLOBO
						//---moto---------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movgloboX, 1.0f, movgloboZ));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(globoRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		moto.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(1.8f, 0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantam.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.8f, 0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantam.RenderModel();


		/*
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movgloboX, 1.0f, movgloboZ));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(globoRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo.RenderModel();

		*/
		
		// tablero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -0.9f, 16.2f));
		model = glm::scale(model, glm::vec3(1.45f, 1.5f, 1.45f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tablero.RenderModel();


//-------------fin del globo---------------------

		//-----se empeza el arco---------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, -0.9f, 18.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		base_arco.RenderModel();

		// Cartel
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0, CartelF, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, CartelF, 0.0f));  // Mover el letrero a su posición
		model = glm::rotate(model, glm::radians(AnguloR), glm::vec3(1.0f, 0.0f, 0.0f));  // Rotar sobre el eje X
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arco.RenderModel();
		/*
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arco.RenderModel();
		*/

		/*
		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche, 0.5f, -3.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
	

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();
		*/


		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
